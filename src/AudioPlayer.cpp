#include "../include/AudioPlayer.h"
#include <boost/thread.hpp>

using namespace std;

AudioPlayer::AudioPlayer(unique_ptr<AudioDeviceFactory> df)
    : device_factory{move(df)} {
  boost::thread t1(&AudioPlayer::main_loop, this);
}

void AudioPlayer::main_loop() {
  while (execute_command()) {
  }
}

bool AudioPlayer::execute_command() {
  if (!message_queue.empty()) {
    cout << 123;
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
    }

    cout << message.command << endl;
    cout << message.audio_file->get_header().get_rate() << endl;
    cout << current_state << endl;

    return false;
  }

  return true;
}

void AudioPlayer::start(unique_ptr<NSFFile> audio_file) {
  device = std::unique_ptr<AudioDevice>(
      device_factory.get()->operator()(audio_file->get_header()));
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
