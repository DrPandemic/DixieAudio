#include "../include/AudioPlayer.h"
#include "../include/NSFFile.h"
#include "../include/PulseaudioDevice.h"
#include <boost/chrono.hpp>
#include <boost/lockfree/queue.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>

using namespace std;

struct PulseFactory : public AudioDeviceFactory {
  AudioDevice *operator()(const AudioHeader &header) override {
    return new PulseaudioDevice(header);
  };
};

int main() {
  ifstream s("../nsf/mario.nsf", ifstream::in | std::ios::binary);
  auto f = make_unique<NSFFile>(s);
  auto factory = PulseFactory{};
  AudioPlayer player{factory};

  player.start(std::move(f));
  boost::this_thread::sleep_for(boost::chrono::seconds{2});
}
