#include "../include/AudioPlayer.h"
#include <random>


using namespace std;
using namespace boost::chrono;
using namespace boost::this_thread;

constexpr const us_t AudioPlayer::BUFFER_US;

namespace {
const int MIN_ARTIFICIAL_LAG = 250;
const int MAX_ARTIFICIAL_LAG = 400;
}

AudioPlayer::AudioPlayer(unique_ptr<AudioDevice> device)
    : device{move(device)}, main_thread{&AudioPlayer::main_loop, this} {
  init();
  log_file.open ("data.txt");
}

void AudioPlayer::init() {
  timing.current_sample_written = 0;
  timing.elapsed_time = us_t(0);
  timing.nb_execution = 1;
  timing.playing_elapsed_time = us_t(0);
  timing.write_us = us_t(0);
  timing.saved_timed_of_first_write = false;
}

void AudioPlayer::main_loop() {
  bool isAlive = true;

  while (isAlive) {
    auto loop_time = duration_cast<us_t>(minuter([&isAlive, this] {
      isAlive = execute_loop();
      ++timing.nb_execution;
    }));
    complete_loop_time.push_back(loop_time);
    timing.elapsed_time += loop_time;

    if (current_state == AudioPlayerState::playing) {
      timing.time_elapsed_since_first_write =
          duration_cast<us_t>(system_clock::now() - timing.time_of_first_write);
      auto estimated_buffer_size =
          (timing.current_sample_written -
           (timing.time_elapsed_since_first_write.count() *
            timing.sample_rate_us));
      auto wanted_buffer_size = BUFFER_US.count() * timing.sample_rate_us;

      if (estimated_buffer_size > wanted_buffer_size) {
        //sleep_time.push_back(us_t(timing.us_per_loop -
        //                         (timing.elapsed_time / timing.nb_execution)));
        auto bef  = system_clock::now();
        sleep_for(us_t(timing.us_per_loop -
                       (timing.elapsed_time / timing.nb_execution)));
        auto after = system_clock::now();
        sleep_time.push_back(duration_cast<us_t>(after-bef) - us_t(timing.us_per_loop -
                                                                    (timing.elapsed_time / timing.nb_execution)));

      }
    }
  }

  log_to_file();
  log_file.close();
}

void AudioPlayer::log_to_file() {
  int max_total = -1;
  int avg_total = 0;
  for (auto u: complete_loop_time){
    avg_total+=u.count();
    if(u.count()> max_total){
      max_total = u.count();
    }
  }

  int max_read = -1;
  int avg_read = 0;
  for (auto u: read_time){
    avg_read+=u.count();
    if(u.count()> max_read){
      max_read = u.count();
    }
  }

  int max_write = -1;
  int avg_write =0;
  for (auto u: write_time){
    avg_write += u.count();
    if(u.count()> max_write){
      max_write = u.count();
    }
  }

  int max_sleep = -1;
  int avg_sleep = 0;
  for (auto u: sleep_time){
    avg_sleep+=u.count();
    if(abs(u.count())> max_sleep){
      max_sleep = abs(u.count());
    }
  }

  log_file << "Max read time = " << max_read << " us\n";
  log_file << "Max write time = " << max_write << " us\n";
  log_file << "Max sleep time = " << max_sleep << " us\n";
  log_file << "Max loop time = " << max_total << " us\n";
  log_file << "avg read time = " << avg_read/read_time.size() << " us\n";
  log_file << "avg write time = " << avg_write/write_time.size() << " us\n";
  log_file << "avg sleep time = " << avg_sleep/sleep_time.size() << " us\n";
  log_file << "avg loop time = " << avg_total/complete_loop_time.size() << " us\n";

}

bool AudioPlayer::execute_loop() {
  if (!execute_command()) {
    return false;
  }

  if (current_state == AudioPlayerState::playing) {
    us_t read_us = timing.us_per_loop - timing.write_us -
                   (timing.elapsed_time / timing.nb_execution);
    read_us = read_us < us_t(0) ? us_t(0) : read_us;

    vector<AudioData> data;
    us_t real_read_us = duration_cast<us_t>(minuter([this, &data, read_us]{data = audio_file->read_while(AudioPlayer::MAX_SAMPLES_PER_LOOP, read_us);}));

    read_time.push_back(real_read_us);

    if (is_lagging) {
      sleep_for(
          us_t(::MIN_ARTIFICIAL_LAG +
               (rand() % (int)(::MAX_ARTIFICIAL_LAG - ::MIN_ARTIFICIAL_LAG))));
    }

    timing.write_us += duration_cast<us_t>(minuter([this, &data] {
      device->write(data);
      if (!timing.saved_timed_of_first_write) {
        timing.time_of_first_write = system_clock::now();
        timing.saved_timed_of_first_write = true;
      }
    }));
    // could use a better function
    timing.write_us /= 2;

    write_time.push_back(timing.write_us);

    // data.size() is the number of char. So, nb_chars * 8 = nb_bits.
    // nb_bits / sample_rate = nb_samples
    timing.current_sample_written +=
        (data.size() * 8 / audio_file->get_header().get_sample_size());

    if (audio_file->eof()) {
      audio_file->restart();
    }
  }
  return true;
}
bool AudioPlayer::execute_command() {
  if (!message_queue.empty()) {
    auto message = message_queue.pull();

    switch (message.command) {
    case (AudioPlayerCommand::start):
      timing.elapsed_time = us_t(0);
      timing.current_sample_written = 0;
      audio_file = std::move(message.audio_file);
      timing.us_per_loop =
          us_t(1'000'000 /
               (audio_file->get_header().get_rate() / MAX_SAMPLES_PER_LOOP));
      timing.sample_rate_us = audio_file->get_header().get_rate() / 1'000'000.;
      current_state = playing;
      break;
    case (AudioPlayerCommand::stop):
      if (current_state == playing) {
        timing.playing_elapsed_time = timing.elapsed_time;
        current_state = paused;
      }
      break;
    case (AudioPlayerCommand::resume):
      if (current_state == paused) {
        timing.elapsed_time = timing.playing_elapsed_time;
        current_state = playing;

        init();
      }
      break;
    case (AudioPlayerCommand::next):
      current_state = playing;
      break;
    case (AudioPlayerCommand::previous):
      current_state = playing;
      break;
    case (AudioPlayerCommand::kill_thread):
      return false;
      break;
    case (AudioPlayerCommand::query_state):
      response_queue.push(Response{current_state});
      break;
    case (AudioPlayerCommand::lag):
      is_lagging = !is_lagging;
      break;
    case (AudioPlayerCommand::query_timing_info):
      Response r;
      r.timing_info = timing;
      response_queue.push(r);
      break;
    }
  }

  return true;
}

void AudioPlayer::start(unique_ptr<AudioFile> audio_file) {
  device->reset_device(audio_file->get_header());
  Message message{AudioPlayerCommand::start};
  message.audio_file = move(audio_file);
  message_queue.push(move(message));
}
void AudioPlayer::stop() {
  Message message{AudioPlayerCommand::stop};
  message_queue.push(move(message));
}
void AudioPlayer::resume() {
  Message message{AudioPlayerCommand::resume};
  message_queue.push(move(message));
}
void AudioPlayer::next() {
  Message message{AudioPlayerCommand::next};
  message_queue.push(move(message));
}
void AudioPlayer::previous() {
  Message message{AudioPlayerCommand::previous};
  message_queue.push(move(message));
}
void AudioPlayer::kill() {
  Message message{AudioPlayerCommand::kill_thread};
  message_queue.push(move(message));
  main_thread.join();
  is_dying = true;
}
AudioPlayerState AudioPlayer::get_state() {
  Message message{AudioPlayerCommand::query_state};
  message_queue.push(move(message));

  Response r;
  response_queue.wait_pull(r);

  return r.state;
}
AudioPlayerTimingInfo AudioPlayer::get_timing_info() {
  Message message{AudioPlayerCommand::query_timing_info};
  message_queue.push(move(message));

  Response r;
  response_queue.wait_pull(r);
  return r.timing_info;
}
void AudioPlayer::toggle_lag() {
  Message message{AudioPlayerCommand::lag};
  message_queue.push(move(message));
}

bool AudioPlayer::is_alive() { return !is_dying; }
