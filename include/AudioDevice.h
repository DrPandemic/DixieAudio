#ifndef DIXIEAUDIO_AUDIO_DEVICE_H
#define DIXIEAUDIO_AUDIO_DEVICE_H

#include "AudioFile.h"
#include <vector>

class AudioDevice {
public:
  AudioDevice(const AudioHeader &){};
  virtual ~AudioDevice(){};
  virtual int write(std::vector<AudioData> data) = 0;
};

#endif // DIXIEAUDIO_AUDIO_DEVICE_H
