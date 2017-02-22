#include "../include/PulseaudioDevice.h"
#include <iostream>

using namespace std;
PulseaudioDevice::PulseaudioDevice(const AudioHeader &header)
    : AudioDevice(header) {

  pa_sample_spec ss;
  ss.format = PA_SAMPLE_U8;
  ss.channels = 1;
  ss.rate = header.get_rate();

  device = pa_simple_new(nullptr,      // Use the default server.
                         "DixieAudio", // Our application's name.
                         PA_STREAM_PLAYBACK,
                         nullptr,              // Use the default device.
                         "Retro-Groovy-Music", // Description of our stream.
                         &ss,                  // Our sample format.
                         nullptr,              // Use default channel map
                         nullptr,          // Use default buffering attributes.
                         &pulse_error_code // Ignore error code.
                         );
}
PulseaudioDevice::~PulseaudioDevice() { pa_simple_free(device); }

int PulseaudioDevice::write(const std::vector<AudioData> data) {
  return pa_simple_write(device, &data[0], 200, nullptr);
}
