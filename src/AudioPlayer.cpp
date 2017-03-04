#include "../include/AudioPlayer.h"

using namespace std;

AudioPlayer::AudioPlayer(unique_ptr<AudioDevice> device)
    : device{move(device)}, main_thread{&AudioPlayer::main_loop, this},
      current_state{AudioPlayerState::stopped} {}

AudioPlayerState AudioPlayer::get_state() const { return current_state; }

void AudioPlayer::main_loop() {
  while (execute_command()) {
  }
}

bool AudioPlayer::execute_command() {
  if (!message_queue.empty()) {
    auto message = message_queue.pull();

    switch (message.command) {
    case (AudioPlayerCommand::start):
      current_state = playing;
    case (AudioPlayerCommand::stop):
      current_state = paused;
    case (AudioPlayerCommand::resume):
      current_state = playing;
    case (AudioPlayerCommand::next):
      current_state = playing;
    case (AudioPlayerCommand::previous):
      current_state = playing;
    case (AudioPlayerCommand::skip_to):
      current_state = playing;
    case (AudioPlayerCommand::kill_thread):
      return false;
    }

    // cout << message.command << endl;
    // cout << message.audio_file->get_header().get_rate() << endl;
    // cout << current_state << endl;
  }

  return true;
}

void AudioPlayer::start(unique_ptr<NSFFile> audio_file) {
  device->reset_device(audio_file->get_header());
  Message message;
  message.command = AudioPlayerCommand::start;
  message.audio_file = move(audio_file);
  message_queue.push(move(message));
}
void AudioPlayer::stop() {}
void AudioPlayer::resume() {}
void AudioPlayer::next() {}
void AudioPlayer::previous() {}
void AudioPlayer::skip_to(int track_id) {}
void AudioPlayer::kill() {
  Message message;
  message.command = AudioPlayerCommand::kill_thread;
  message_queue.push(move(message));
  main_thread.join();
}
