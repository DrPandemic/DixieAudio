#include "../include/pulseaudio_device.h"

pulseaudio_device::pulseaudio_device(audio_header header)
    : audio_device(header) {}
pulseaudio_device::~pulseaudio_device() {}

int pulseaudio_device::write(std::vector<audio_data> data) {
  return 0; // temp
}
