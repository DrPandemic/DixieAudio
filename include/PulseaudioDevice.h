#ifndef DIXIEAUDIO_PULSEAUDIO_DEVICE_H
#define DIXIEAUDIO_PULSEAUDIO_DEVICE_H

#include "AudioDevice.h"
#include "AudioFile.h"
#include <memory>
#include <pulse/simple.h>

class PulseaudioDevice : public AudioDevice {
public:
  PulseaudioDevice();
  PulseaudioDevice(const AudioHeader &);
  ~PulseaudioDevice() override;
  void reset_device(const AudioHeader &) override;

  int write(const std::vector<AudioData> data) override;

private:
  std::unique_ptr<pa_simple, decltype(&pa_simple_free)> device;
  int pulse_error_code;
};

#endif // DIXIEAUDIO_PULSEAUDIO_DEVICE_H
