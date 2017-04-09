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
  kill_thread,
  query_state,
  query_timing_info,
  lag
};

struct AudioPlayerTimingInfo {
  size_t current_sample_written;
  us_t elapsed_time;
  size_t nb_execution;
  us_t playing_elapsed_time;
  us_t write_us;
  us_t time_elapsed_since_first_write;
  double sample_rate_us;
  time_point_t time_of_first_write;
  bool saved_timed_of_first_write;
  us_t us_per_loop;
};

struct Message {
  AudioPlayerCommand command;
  std::unique_ptr<AudioFile> audio_file;
  Message(AudioPlayerCommand c) : command{c} {};
  Message(){};
};

struct Response {
  AudioPlayerTimingInfo timing_info;
  AudioPlayerState state;
  Response(AudioPlayerState s) : state{s} {};
  Response(AudioPlayerTimingInfo t) : timing_info{t} {};
  Response(){};
};

class AudioPlayer {
private:
  static const size_t MAX_SAMPLES_PER_LOOP = 4;

  std::unique_ptr<AudioDevice> device;
  std::unique_ptr<AudioFile> audio_file;
  boost::sync_queue<Message> message_queue;
  boost::sync_queue<Response> response_queue;
  boost::thread main_thread;

  AudioPlayerState current_state = stopped;
  bool is_lagging = false;
  bool is_dying = false;
  AudioPlayerTimingInfo timing;

  void main_loop();
  bool execute_command();
  bool execute_loop();

public:
  constexpr static const us_t BUFFER_US = us_t(10000);

  AudioPlayer(std::unique_ptr<AudioDevice>);
  void init();
  void start(std::unique_ptr<AudioFile> audio_file);
  void stop();
  void resume();
  void next();
  void previous();
  void kill();
  bool is_alive();
  void toggle_lag();

  AudioPlayerState get_state();
  AudioPlayerTimingInfo get_timing_info();
};

#endif // DIXIEAUDIO_AUDIO_PLAYER_H
