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
  auto s = make_unique<ifstream>("../music/mario_09.wav",
                                 ifstream::in | std::ios::binary);
  auto f = make_unique<WAVFile>(move(s));
  unique_ptr<AudioDevice> device = make_unique<PulseaudioDevice>();
  AudioPlayer player{move(device)};
  player.start(std::move(f));

  while (player.is_alive()) {

    if (std::cin.rdbuf() and std::cin.rdbuf()->in_avail() >= 0) {

      char user_cmd;
      cin >> user_cmd;
      cout << user_cmd << endl;

      switch (user_cmd) {
      case 's':
        player.resume();
        break; // prints "1",
      case 'p':
        player.stop();
        break; // then prints "2"
      }
    }
  }

  // boost::this_thread::sleep( boost::posix_time::seconds(5) );
  // player.stop();
  // boost::this_thread::sleep( boost::posix_time::seconds(5) );
  // player.resume();
  // boost::this_thread::sleep_for(boost::chrono::seconds{10});
}
