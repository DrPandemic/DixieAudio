//
// Created by bugz on 07/02/17.
//

#ifndef DIXIEAUDIO_AUDIO_DEVICE_H
#define DIXIEAUDIO_AUDIO_DEVICE_H

#include "./audio_file.h"
#include <vector>

class audio_device {
public:
  audio_device(audio_header header);
  virtual ~audio_device();
  virtual int write(std::vector<audio_data> data);
};

#endif // DIXIEAUDIO_AUDIO_DEVICE_H
