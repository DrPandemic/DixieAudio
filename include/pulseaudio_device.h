//
// Created by bugz on 07/02/17.
//

#ifndef DIKIEAUDIO_PULSEAUDIO_DEVICE_H
#define DIKIEAUDIO_PULSEAUDIO_DEVICE_H

#include "audio_device.h"
#include "audio_file.h"

class pulseaudio_device : public audio_device {
public:
  pulseaudio_device(audio_header header) override;
  ~pulseaudio_device() override;

  int write(vector<audio_data> data) override;
};

#endif // DIKIEAUDIO_PULSEAUDIO_DEVICE_H
