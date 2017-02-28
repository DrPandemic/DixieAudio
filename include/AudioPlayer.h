#ifndef DIXIEAUDIO_AUDIO_PLAYER_H
#define DIXIEAUDIO_AUDIO_PLAYER_H

#include "AudioDevice.h"
#include "NSFFile.h"
#include "PulseaudioDevice.h"
#include <boost/thread/concurrent_queues/sync_queue.hpp>
#include <memory>

enum AudioPlayerState { playing, stopped, paused };
enum AudioPlayerCommand {
  start,
  stop,
  resume,
  next,
  previous,
  skip_to,
};

struct Message {
  AudioPlayerCommand command;
  std::unique_ptr<AudioFile> audio_file;
  int skip_to_track_id;
};

template <class DeviceType> class AudioPlayer {
private:
  std::unique_ptr<DeviceType> device;
  std::unique_ptr<AudioFile> audio_file;
  boost::sync_queue<Message> message_queue;
  int current_song;
  AudioPlayerState current_state = stopped;

  void main_loop();
  bool execute_command();

public:
  AudioPlayer();
  void start(std::unique_ptr<NSFFile> audio_file);
  void stop();
  void resume();
  void next();
  void previous();
  void skip_to(int track_id);
};

#endif // DIXIEAUDIO_AUDIO_PLAYER_H
