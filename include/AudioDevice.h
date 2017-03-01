#ifndef DIXIEAUDIO_AUDIO_DEVICE_H
#define DIXIEAUDIO_AUDIO_DEVICE_H

#include "AudioFile.h"
#include <vector>

class AudioDevice {
public:
  AudioDevice(){};
  AudioDevice(const AudioHeader &){};
  virtual ~AudioDevice(){};
  virtual int write(const std::vector<AudioData> data) = 0;
  virtual void reset_device(const AudioHeader &header) = 0;
};

#endif // DIXIEAUDIO_AUDIO_DEVICE_H
