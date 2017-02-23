#include "../include/AudioPlayer.h"
#include <boost/thread.hpp>

using namespace std;

AudioPlayer::AudioPlayer(std::unique_ptr<PulseaudioDevice> audio_device)
    : device{std::move(audio_device)} {
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

    cout << message.command;
    cout << message.audio_file->get_header().get_rate() << endl;

    return false;
  }

  return true;
}

void AudioPlayer::start(unique_ptr<NSFFile> audio_file) {
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
