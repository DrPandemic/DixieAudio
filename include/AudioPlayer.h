#ifndef DIXIEAUDIO_AUDIO_PLAYER_H
#define DIXIEAUDIO_AUDIO_PLAYER_H

#include "../include/Minuter.h"
#include "AudioDevice.h"
#include "WAVFile.h"
#include <boost/chrono.hpp>
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
  boost::chrono::microseconds micro_per_loop;

  static const size_t MAX_SAMPLES_PER_LOOP = 4;

  int current_song;
  AudioPlayerState current_state = stopped;
  bool resample = false;
  size_t new_rate = 0;

  void main_loop();
  bool execute_command();
  bool execute_loop();

  size_t current_sample_written = 0;
  us_t elapsed_time = us_t(0);
  size_t nb_execution = 0;
  us_t playing_elapsed_time = us_t(0);
  us_t write_us = us_t(0);
  us_t resample_us = us_t(0);

  // buffering
  time_point_t time_of_first_write;
  bool saved_timed_of_first_write = false;
  constexpr static const us_t BUFFER_US = us_t(10000);

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
