#ifndef DIXIEAUDIO_AUDIO_PLAYER_H
#define DIXIEAUDIO_AUDIO_PLAYER_H

#include "NSFFile.h"
#include "PulseaudioDevice.h"
#include <memory>

class AudioPlayer {
private:
  // thread_safe_queue HUGO

public:
  AudioPlayer(std::unique_ptr<PulseaudioDevice> audio_device);
  void start(std::unique_ptr<NSFFile> audio_file);
  void stop();
  void resume();
  void next();
  void previous();
  void skip_to(int track_id);
};

#endif // DIXIEAUDIO_AUDIO_PLAYER_H
