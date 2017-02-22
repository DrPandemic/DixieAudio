#ifndef DIXIEAUDIO_PULSEAUDIO_DEVICE_H
#define DIXIEAUDIO_PULSEAUDIO_DEVICE_H

#include "AudioDevice.h"
#include "AudioFile.h"
#include <memory>
#include <pulse/simple.h>

class PulseaudioDevice : public AudioDevice {
public:
  PulseaudioDevice(AudioHeader header);
  ~PulseaudioDevice() override;

  int write(std::vector<AudioData> data) override;

private:
  pa_simple *device;
};

#endif // DIXIEAUDIO_PULSEAUDIO_DEVICE_H
