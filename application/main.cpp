#include "../include/AudioPlayer.h"
#include "../include/PulseaudioDevice.h"
#include "../include/WAVFile.h"
#include <boost/chrono.hpp>
#include <boost/lockfree/queue.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>

using namespace std;

int main() {
  ifstream s("../music/mario_09.wav", ifstream::in | std::ios::binary);
  auto f = make_unique<WAVFile>(s);
  auto file = *f;
  cout << file;
  unique_ptr<AudioDevice> device = make_unique<PulseaudioDevice>();
  AudioPlayer player{move(device)};

  // player.start(std::move(f));
  // boost::this_thread::sleep_for(boost::chrono::seconds{2});
}
