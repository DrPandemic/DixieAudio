//
// Created by bugz on 07/02/17.
//

#ifndef DIKIEAUDIO_AUDIO_PLAYER_H
#define DIKIEAUDIO_AUDIO_PLAYER_H

#include "NSF_file.h"
#include "pulseaudio_device.h"
#include <memory>

class audio_player {
private:
  // thread_safe_queue HUGO

public:
  audio_player(unique_ptr<pulseaudio_device> audio_device);
  void start(unique_ptr<NSF_file> audio_file);
  void stop();
  void resume();
  void next();
  void previous();
  void skip_to(int track_id);
};

#endif // DIKIEAUDIO_AUDIO_PLAYER_H
