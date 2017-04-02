#ifndef DIXIEAUDIO_AUDIO_PLAYER_H
#define DIXIEAUDIO_AUDIO_PLAYER_H

#include "AudioDevice.h"
#include "WAVFile.h"
#include <boost/thread.hpp>
#include <boost/thread/concurrent_queues/sync_queue.hpp>
#include <memory>
#include "../include/Minuter.h"

enum AudioPlayerState { playing, stopped, paused };
enum AudioPlayerCommand {
  start,
  stop,
  resume,
  next,
  previous,
  skip_to,
  kill_thread,
  query_state,
  downsample
};

struct Message {
  AudioPlayerCommand command;
  std::unique_ptr<AudioFile> audio_file;
  int skip_to_track_id;
  Message(AudioPlayerCommand c) : command{c} {};
  Message(){};
};

struct Response {
  AudioPlayerState state;
  Response(AudioPlayerState s) : state{s} {};
  Response(){};
};

class AudioPlayer {
private:
  std::unique_ptr<AudioDevice> device;
  std::unique_ptr<AudioFile> audio_file;
  boost::sync_queue<Message> message_queue;
  boost::sync_queue<Response> response_queue;
  boost::thread main_thread;
  double buffer_sample;
  std::chrono::microseconds micro_per_loop;

  static const size_t MAX_SAMPLES_PER_LOOP = 4;
  constexpr static const std::chrono::microseconds BUFFER_MICROS = std::chrono::microseconds(100000);

  int current_song;
  AudioPlayerState current_state = stopped;
  bool resample = false;
  size_t new_rate = 0;

  void main_loop();
  bool execute_command();
  bool execute_loop();
  size_t current_sample_written = 0;
  std::chrono::microseconds elapsed_time = std::chrono::microseconds(0);
  std::chrono::microseconds playing_elapsed_time = std::chrono::microseconds(0);

  bool is_dying = false;

public:
  AudioPlayer(std::unique_ptr<AudioDevice>);
  void start(std::unique_ptr<AudioFile> audio_file);
  void stop();
  void resume();
  void next();
  void previous();
  void skip_to(int track_id);
  void kill();
  bool is_alive();
  void downsample();
  std::vector<AudioData> resample_data(std::vector<AudioData>);

  AudioPlayerState get_state();
};

#endif // DIXIEAUDIO_AUDIO_PLAYER_H
