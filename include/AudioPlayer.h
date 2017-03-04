#ifndef DIXIEAUDIO_AUDIO_PLAYER_H
#define DIXIEAUDIO_AUDIO_PLAYER_H

#include "AudioDevice.h"
#include "NSFFile.h"
#include <atomic>
#include <boost/thread.hpp>
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
  kill_thread,
};

struct Message {
  AudioPlayerCommand command;
  std::unique_ptr<AudioFile> audio_file;
  int skip_to_track_id;
};

class AudioPlayer {
private:
  std::unique_ptr<AudioDevice> device;
  std::unique_ptr<AudioFile> audio_file;
  boost::sync_queue<Message> message_queue;
  boost::thread main_thread;

  int current_song;
  std::atomic<AudioPlayerState> current_state;

  void main_loop();
  bool execute_command();

public:
  AudioPlayer(std::unique_ptr<AudioDevice>);
  void start(std::unique_ptr<NSFFile> audio_file);
  void stop();
  void resume();
  void next();
  void previous();
  void skip_to(int track_id);
  void kill();

  AudioPlayerState get_state() const;
};

#endif // DIXIEAUDIO_AUDIO_PLAYER_H
