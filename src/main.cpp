#include <iostream>
#include <pulse/error.h>
#include <pulse/simple.h>

using namespace std;

int main() {
  pa_simple *s;
  pa_sample_spec ss;
  ss.format = PA_SAMPLE_S16NE;
  ss.channels = 2;
  ss.rate = 44100;
  s = pa_simple_new(NULL,     // Use the default server.
                    "Fooapp", // Our application's name.
                    PA_STREAM_PLAYBACK,
                    NULL,    // Use the default device.
                    "Music", // Description of our stream.
                    &ss,     // Our sample format.
                    NULL,    // Use default channel map
                    NULL,    // Use default buffering attributes.
                    NULL     // Ignore error code.
                    );
  cout << "HELLO JS" << endl;
  cout << "HELLO JS" << endl;
}
