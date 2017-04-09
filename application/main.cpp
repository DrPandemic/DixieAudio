#include "../include/AudioPlayer.h"
#include "../include/PulseaudioDevice.h"
#include <fstream>
#include <termios.h>

using namespace std;

// This is not portable. I guess pulse is also not portable...
void stop_buffering() {
  struct termios t;
  tcgetattr(STDIN_FILENO, &t);
  t.c_lflag &= ~ICANON;
  tcsetattr(STDIN_FILENO, TCSANOW, &t);

  cin.sync_with_stdio(false);
}

void display_ui(const WAVHeader &header) {
  cout << "Keys:" << endl;
  cout << "   s: start" << endl;
  cout << "   p: pause" << endl;
  cout << "   x: stop" << endl;
  cout << "   d: downsample" << endl << endl;
  cout << "File Information:" << endl;
  cout << "Number of channels: " << header.number_of_channels << endl;
  cout << "Sample rate: " << header.sample_rate << " Hz" << endl;
  cout << "Data size: " << header.data_size << " bytes" << endl;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cout << "Usage: " << argv[0] << " <WAV_file_path>" << endl;
    return EXIT_FAILURE;
  }

  string file_path(argv[1]);
  auto s = make_unique<ifstream>(file_path, ifstream::in | std::ios::binary);

  // if the file is open, we can start to play
  if (s->is_open()) {
    auto f = make_unique<WAVFile>(move(s));
    unique_ptr<AudioDevice> device = make_unique<PulseaudioDevice>();
    auto header = f->get_header();

    // only supporting 16 bits for now
    if (header.is_valid()) {
      AudioPlayer player{move(device)};
      player.start(std::move(f));

      stop_buffering();
      display_ui(header);

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
        case 'd':
          player.downsample();
          break;
        default:
          cout << "\n"
               << "Valid commands are : " << endl;
          cout << "p : pause" << endl;
          cout << "s : resume" << endl;
          cout << "x : stop" << endl;
          break;
        }
      }
    } else {
      cout << "Sorry we only support 16bits for now " << endl;
      return EXIT_FAILURE;
    }
  } else {
    cout << file_path << " is invalid " << endl;
    return EXIT_FAILURE;
  }
}
