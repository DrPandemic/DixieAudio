#ifndef DIXIEAUDIO_AUDIO_DEVICE_H
#define DIXIEAUDIO_AUDIO_DEVICE_H

#include "./audio_file.h"
#include <vector>

class audio_device {
protected:
  audio_header header;

public:
  audio_device(audio_header header) : header{header} {};
  virtual ~audio_device(){};
  virtual int write(std::vector<audio_data> data) = 0;
};

#endif // DIXIEAUDIO_AUDIO_DEVICE_H
