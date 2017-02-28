#ifndef DIXIEAUDIO_PULSEAUDIO_DEVICE_H
#define DIXIEAUDIO_PULSEAUDIO_DEVICE_H

#include "AudioDevice.h"
#include "AudioFile.h"
#include <memory>
#include <pulse/simple.h>

class PulseaudioDevice : public AudioDevice {
public:
  PulseaudioDevice(const AudioHeader &);
  ~PulseaudioDevice() override;

  int write(const std::vector<AudioData> data) override;

private:
  pa_simple *device;
  int pulse_error_code;
};

struct PulseFactory : public AudioDeviceFactory {
  AudioDevice *operator()(const AudioHeader &header) override {
    return new PulseaudioDevice(header);
  };
};

#endif // DIXIEAUDIO_PULSEAUDIO_DEVICE_H
