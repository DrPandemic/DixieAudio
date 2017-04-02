#include "../include/AudioPlayer.h"

using namespace std;

constexpr const chrono::microseconds AudioPlayer::BUFFER_MICROS;

AudioPlayer::AudioPlayer(unique_ptr<AudioDevice> device)
    : device{move(device)}, main_thread{&AudioPlayer::main_loop, this} {}

void AudioPlayer::main_loop() {
  while (execute_command()) {
    bool isAlive = true;
    auto time = minuter([&isAlive, this] { isAlive = execute_loop(); });
    // cout << time.count() << endl;
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
    device->write(data);

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
      cout << "ICIIII" << endl;
      audio_file = std::move(message.audio_file);
      buffer_sample =
          BUFFER_MICROS.count() * audio_file->get_header().get_rate();
      micro_per_loop = std::chrono::microseconds(
          1'000'000 / audio_file->get_header().get_rate() /
          MAX_SAMPLES_PER_LOOP);
        cout << "yolo" << buffer_sample << endl;
        cout << micro_per_loop.count() << endl;
      current_state = playing;
      break;
    case (AudioPlayerCommand::stop):
      current_state = paused;
      break;
    case (AudioPlayerCommand::resume):
      current_state = playing;
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

bool AudioPlayer::is_alive() { return !is_dying; }
