#ifndef DIXIEAUDIO_AUDIO_DEVICE_H
#define DIXIEAUDIO_AUDIO_DEVICE_H

#include "AudioFile.h"
#include <vector>

class AudioDevice {
protected:
  audio_header header;

public:
  AudioDevice(audio_header header) : header{header} {};
  virtual ~AudioDevice(){};
  virtual int write(std::vector<audio_data> data) = 0;
};

#endif // DIXIEAUDIO_AUDIO_DEVICE_H
