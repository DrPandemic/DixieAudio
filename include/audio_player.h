#ifndef DIXIEAUDIO_AUDIO_PLAYER_H
#define DIXIEAUDIO_AUDIO_PLAYER_H

#include "NSF_file.h"
#include "pulseaudio_device.h"
#include <memory>

class audio_player {
private:
  // thread_safe_queue HUGO

public:
  audio_player(std::unique_ptr<pulseaudio_device> audio_device);
  void start(std::unique_ptr<NSF_file> audio_file);
  void stop();
  void resume();
  void next();
  void previous();
  void skip_to(int track_id);
};

#endif // DIXIEAUDIO_AUDIO_PLAYER_H
