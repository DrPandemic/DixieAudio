#include "../include/PulseaudioDevice.h"

using namespace std;
PulseaudioDevice::PulseaudioDevice(AudioHeader header)
    : AudioDevice(header){


  pa_sample_spec ss;
  ss.format = PA_SAMPLE_U8;
  ss.channels = 1;
  ss.rate = header.get_rate();
//  device = pa_simple_new(NULL,     // Use the default server.
//                    "Fooapp", // Our application's name.
//                    PA_STREAM_PLAYBACK,
//                    NULL,    // Use the default device.
//                    "Music", // Description of our stream.
//                    &ss,     // Our sample format.
//                    NULL,    // Use default channel map
//                    NULL,    // Use default buffering attributes.
//                    NULL,    // Ignore error code.
//                    );
}
PulseaudioDevice::~PulseaudioDevice() {}

int PulseaudioDevice::write(std::vector<AudioData> data) {
  return 0; // temp
}
