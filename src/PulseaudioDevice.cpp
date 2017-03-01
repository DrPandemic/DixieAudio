#include "../include/PulseaudioDevice.h"
#include "../include/NSFFile.h"
#include <iostream>

using namespace std;

PulseaudioDevice::PulseaudioDevice() : device(nullptr, &pa_simple_free) {

  pa_sample_spec ss;
  ss.format = PA_SAMPLE_U8;
  ss.channels = 1;
  ss.rate = 44100;

  device.reset(pa_simple_new(nullptr,      // Use the default server.
                             "DixieAudio", // Our application's name.
                             PA_STREAM_PLAYBACK,
                             nullptr,              // Use the default device.
                             "Retro-Groovy-Music", // Description of our stream.
                             &ss,                  // Our sample format.
                             nullptr,              // Use default channel map
                             nullptr, // Use default buffering attributes.
                             &pulse_error_code)); // Ignore error code.
}

PulseaudioDevice::PulseaudioDevice(const AudioHeader &header)
    : AudioDevice(header), device(nullptr, &pa_simple_free) {

  reset_device(header);
}
PulseaudioDevice::~PulseaudioDevice() {}

void PulseaudioDevice::reset_device(const AudioHeader &header) {

  pa_sample_spec ss;
  ss.format = PA_SAMPLE_U8; // in header?
  ss.channels = 1;          // in header?
  ss.rate = header.get_rate();

  device.reset(pa_simple_new(nullptr,      // Use the default server.
                             "DixieAudio", // Our application's name.
                             PA_STREAM_PLAYBACK,
                             nullptr,              // Use the default device.
                             "Retro-Groovy-Music", // Description of our stream.
                             &ss,                  // Our sample format.
                             nullptr,              // Use default channel map
                             nullptr, // Use default buffering attributes.
                             &pulse_error_code)); // Ignore error code.
}

int PulseaudioDevice::write(const std::vector<AudioData> data) {
  return pa_simple_write(device.get(), &data[0], 200, nullptr);
}
