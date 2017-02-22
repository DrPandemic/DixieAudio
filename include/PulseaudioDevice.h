#ifndef DIXIEAUDIO_PULSEAUDIO_DEVICE_H
#define DIXIEAUDIO_PULSEAUDIO_DEVICE_H

#include "AudioDevice.h"
#include "AudioFile.h"
#include <memory>
#include <pulse/simple.h>

class PulseaudioDevice : public AudioDevice {
public:
  PulseaudioDevice(audio_header header);
  ~PulseaudioDevice() override;

  int write(std::vector<audio_data> data) override;

private:
  pa_simple *device;
};

#endif // DIXIEAUDIO_PULSEAUDIO_DEVICE_H
