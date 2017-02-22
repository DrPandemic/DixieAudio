#ifndef DIXIEAUDIO_AUDIO_DEVICE_H
#define DIXIEAUDIO_AUDIO_DEVICE_H

#include "AudioFile.h"
#include <vector>

class AudioDevice {
protected:
  AudioHeader header;

public:
  AudioDevice(AudioHeader header) : header{header} {};
  virtual ~AudioDevice(){};
  virtual int write(std::vector<AudioData> data) = 0;
};

#endif // DIXIEAUDIO_AUDIO_DEVICE_H
