#include "../include/AudioPlayer.h"
#include <random>

using namespace std;
using namespace boost::chrono;

constexpr const us_t AudioPlayer::BUFFER_US;

AudioPlayer::AudioPlayer(unique_ptr<AudioDevice> device)
    : device{move(device)}, main_thread{&AudioPlayer::main_loop, this} {
  init();
}

void AudioPlayer::init() {
  timing_info.current_sample_written = 0;
  timing_info.elapsed_time = us_t(0);
  timing_info.nb_execution = 1;
  timing_info.playing_elapsed_time = us_t(0);
  timing_info.write_us = us_t(0);
  timing_info.saved_timed_of_first_write = false;
}

void AudioPlayer::main_loop() {
  bool isAlive = true;

  while (isAlive) {
    timing_info.elapsed_time += duration_cast<us_t>(minuter([&isAlive, this] {
      isAlive = execute_loop();
      ++timing_info.nb_execution;
    }));

    if (current_state == AudioPlayerState::playing) {
      timing_info.time_elapsed_since_first_write = duration_cast<us_t>(
          system_clock::now() - timing_info.time_of_first_write);
      auto estimated_buffer_size =
          (timing_info.current_sample_written -
           (timing_info.time_elapsed_since_first_write.count() *
            timing_info.sample_rate_us));
      auto wanted_buffer_size = BUFFER_US.count() * timing_info.sample_rate_us;

      if (estimated_buffer_size > wanted_buffer_size) {
        boost::this_thread::sleep_for(
            us_t(micro_per_loop -
                 (timing_info.elapsed_time / timing_info.nb_execution)));
      }
    }
  }
}

bool AudioPlayer::execute_loop() {
  bool playerIsAlive = execute_command();

  if (!playerIsAlive) {
    return false;
  }

  if (current_state == AudioPlayerState::playing) {
    us_t read_us = micro_per_loop - timing_info.write_us -
                   (timing_info.elapsed_time / timing_info.nb_execution);
    read_us = read_us < us_t(0) ? us_t(0) : read_us;

    vector<AudioData> data =
        audio_file->read_while(AudioPlayer::MAX_SAMPLES_PER_LOOP, read_us);

    if (is_lagging) {
      boost::this_thread::sleep_for(
          us_t(250 + (rand() % (int)(400 - 250 + 1))));
    }

    timing_info.write_us += duration_cast<us_t>(minuter([this, &data] {
      device->write(data);
      if (!timing_info.saved_timed_of_first_write) {
        timing_info.time_of_first_write = system_clock::now();
        timing_info.saved_timed_of_first_write = true;
      }
    }));
    // could use a better function
    timing_info.write_us /= 2;

    // data.size() is the number of char. So, nb_chars * 8 = nb_bits.
    // nb_bits / sample_rate = nb_samples
    timing_info.current_sample_written +=
        (data.size() * 8 / audio_file->get_header().get_sample_size());

    if (audio_file->eof()) {
      audio_file->restart();
    }
  }
  return playerIsAlive;
}
bool AudioPlayer::execute_command() {
  if (!message_queue.empty()) {
    auto message = message_queue.pull();

    switch (message.command) {
    case (AudioPlayerCommand::start):
      timing_info.elapsed_time = us_t(0);
      timing_info.current_sample_written = 0;
      audio_file = std::move(message.audio_file);
      micro_per_loop = us_t(1'000'000 / (audio_file->get_header().get_rate() /
                                         MAX_SAMPLES_PER_LOOP));
      timing_info.sample_rate_us =
          audio_file->get_header().get_rate() / 1'000'000.;
      current_state = playing;
      break;
    case (AudioPlayerCommand::stop):
      if (current_state == playing) {
        timing_info.playing_elapsed_time = timing_info.elapsed_time;
        current_state = paused;
      }
      break;
    case (AudioPlayerCommand::resume):
      if (current_state == paused) {
        timing_info.elapsed_time = timing_info.playing_elapsed_time;
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
    case (AudioPlayerCommand::skip_to):
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
      r.audio_player_timing_info = timing_info;
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
void AudioPlayer::skip_to(int track_id) {
  Message message{AudioPlayerCommand::previous};
  message.skip_to_track_id = track_id;
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

  return r.audio_player_timing_info;
}
void AudioPlayer::toggle_lag() {
  Message message{AudioPlayerCommand::lag};
  message_queue.push(move(message));
}

bool AudioPlayer::is_alive() { return !is_dying; }
