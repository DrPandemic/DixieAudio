#include "../include/AudioPlayer.h"

using namespace std;

constexpr const chrono::microseconds AudioPlayer::BUFFER_MICROS;

AudioPlayer::AudioPlayer(unique_ptr<AudioDevice> device)
    : device{move(device)}, main_thread{&AudioPlayer::main_loop, this} {}

void AudioPlayer::main_loop() {
  bool isAlive = true;

  while (isAlive) {
    elapsed_time += duration_cast<microseconds>(
        minuter([&isAlive, this] { isAlive = execute_loop(); }));
    // cout << "ELAPSED time: " << elapsed_time.count() << endl;

    if (audio_file and current_state == playing) {

      auto sample_rate = audio_file->get_header().get_rate();
      auto played_sample = (sample_rate * elapsed_time) / 1'000'000;

      // cout << "Playing Ellapsed Time" << playing_elapsed_time.count();
      // cout << "elapsed time: " << elapsed_time.count() << endl;
      // cout << " number of sample played: (theorically) " <<
      // played_sample.count() << endl;
    }
  }
}

bool AudioPlayer::execute_loop() {

  bool playerIsAlive = execute_command();

  if (!playerIsAlive) {
    return false;
  }

  if (current_state == AudioPlayerState::playing) {

    vector<AudioData> data = audio_file->read_while(
        AudioPlayer::MAX_SAMPLES_PER_LOOP, micro_per_loop);

    if (resample) {
      cout << "before" << data[0] << endl;
      data = resample_data(data);
      cout << "after" << data[0] << endl;
    }

    current_sample_written += 4;
    device->write(data);

    if (audio_file->eof()) {
      audio_file->restart();
      elapsed_time = chrono::microseconds(0);
      current_sample_written = 0;
    }
  }
  return playerIsAlive;
}
bool AudioPlayer::execute_command() {
  if (!message_queue.empty()) {
    auto message = message_queue.pull();

    switch (message.command) {
    case (AudioPlayerCommand::start):
      elapsed_time = chrono::microseconds(0);
      current_sample_written = 0;
      audio_file = std::move(message.audio_file);
      buffer_sample =
          (BUFFER_MICROS.count() * audio_file->get_header().get_rate()) /
          1'000'000;
      micro_per_loop = std::chrono::microseconds(
          1'000'000 /
          (audio_file->get_header().get_rate() / MAX_SAMPLES_PER_LOOP));
      new_rate = audio_file->get_header().get_rate();
      current_state = playing;
      break;
    case (AudioPlayerCommand::stop):
      if (current_state == playing) {
        playing_elapsed_time = elapsed_time;
        current_state = paused;
      }
      break;
    case (AudioPlayerCommand::resume):
      if (current_state == paused) {
        elapsed_time = playing_elapsed_time;
        current_state = playing;
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
    case (AudioPlayerCommand::downsample):
      resample = true;
      new_rate = new_rate / 2;
      // TODO
      break;
    case (AudioPlayerCommand::kill_thread):
      return false;
      break;
    case (AudioPlayerCommand::query_state):
      response_queue.push(Response{current_state});
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
void AudioPlayer::downsample() {
  Message message{AudioPlayerCommand::downsample};
  message_queue.push(move(message));
  // TODO
  return;
}
AudioPlayerState AudioPlayer::get_state() {
  Message message{AudioPlayerCommand::query_state};
  message_queue.push(move(message));

  Response r;
  response_queue.wait_pull(r);

  return r.state;
}

bool AudioPlayer::is_alive() { return !is_dying; }

std::vector<AudioData> AudioPlayer::resample_data(std::vector<AudioData> data) {
  char first_number[] = {0, 0};
  char second_number[] = {0, 0};
  int value;
  std::vector<AudioData> newData;
  newData.resize(4);

  for (int i = 0; i < newData.size()/2; i ++) {
    first_number[i] = data[i*4];
    first_number[i + 1] = data[i*4 + 1];
    second_number[i] = data[i*4 + 2];
    second_number[i + 1] = data[i*4 + 3];

    uint16_t f_num = *((int*) first_number);
    uint16_t s_num = *((int*) second_number);
    cout << "f_num " << endl;
    cout << f_num  << endl;
    cout << "s_num " << endl;
    cout << s_num << endl;

    uint16_t interp = (f_num + s_num) / 2;

    //TODO figure how to retrieve two char from this int representation.

   // newData[i*2] = c[0];
   // newData[i*2 + 1] = c[1];
    // cout << newData[i] << endl;
  }
  return newData;
}
