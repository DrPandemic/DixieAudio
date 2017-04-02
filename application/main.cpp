#include "../include/AudioPlayer.h"
#include "../include/PulseaudioDevice.h"
#include "../include/WAVFile.h"
#include <boost/chrono.hpp>
#include <boost/lockfree/queue.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <termios.h>
#include "../include/Minuter.h"

using namespace std;

// This is reallllllly not portable. I guess pulse is also not portable...
void stop_buffering() {
  struct termios t;
  tcgetattr(STDIN_FILENO, &t);
  t.c_lflag &= ~ICANON;
  tcsetattr(STDIN_FILENO, TCSANOW, &t);

  cin.sync_with_stdio(false);
}

int main() {
  auto s = make_unique<ifstream>("../music/mario_09.wav",
                                 ifstream::in | std::ios::binary);
  auto f = make_unique<WAVFile>(move(s));
  unique_ptr<AudioDevice> device = make_unique<PulseaudioDevice>();
  AudioPlayer player{move(device)};
  player.start(std::move(f));

  stop_buffering();

  while (player.is_alive()) {

      char user_cmd = getchar();

      switch (user_cmd) {
      case 's':
        player.resume();
        break;
      case 'p':
        player.stop();
        break;
      case 'x':
        player.kill();
        break;
      default:
        cout << "Invalid command, the valid commands are:" << endl;
        cout << "s : start" << endl;
        cout << "p : pause" << endl;
        cout << "x : kill" << endl;
        break;
      }
    }

}
