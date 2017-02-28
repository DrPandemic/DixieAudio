#include "../include/AudioPlayer.h"
#include <boost/thread.hpp>

using namespace std;

template <class DeviceType> AudioPlayer<DeviceType>::AudioPlayer() {
  boost::thread t1(&AudioPlayer<DeviceType>::main_loop, this);
}

template <class DeviceType> void AudioPlayer<DeviceType>::main_loop() {
  while (execute_command()) {
  }
}

template <class DeviceType> bool AudioPlayer<DeviceType>::execute_command() {
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

template <class DeviceType>
void AudioPlayer<DeviceType>::start(unique_ptr<NSFFile> audio_file) {

  device = make_unique<DeviceType>(audio_file->get_header());
  Message message;
  message.command = AudioPlayerCommand::start;
  message.audio_file = move(audio_file);
  message_queue.push(move(message));
}
template <class DeviceType> void AudioPlayer<DeviceType>::stop() {}
template <class DeviceType> void AudioPlayer<DeviceType>::resume() {}
template <class DeviceType> void AudioPlayer<DeviceType>::next() {}
template <class DeviceType> void AudioPlayer<DeviceType>::previous() {}
template <class DeviceType>
void AudioPlayer<DeviceType>::skip_to(int track_id) {}
