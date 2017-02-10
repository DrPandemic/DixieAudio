//
// Created by bugz on 07/02/17.
//

#ifndef DIXIEAUDIO_PULSEAUDIO_DEVICE_H
#define DIXIEAUDIO_PULSEAUDIO_DEVICE_H

#include "audio_device.h"
#include "audio_file.h"

class pulseaudio_device : public audio_device {
public:
  pulseaudio_device(audio_header header);
  ~pulseaudio_device() override;

  int write(std::vector<audio_data> data) override;
};

#endif // DIXIEAUDIO_PULSEAUDIO_DEVICE_H
