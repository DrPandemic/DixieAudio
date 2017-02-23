#include "../include/NSFFile.h"
#include "../include/PulseaudioDevice.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include<boost/lockfree/queue.hpp>

using namespace std;

int main() {
  ifstream s("../nsf/mario.nsf", ifstream::in | std::ios::binary);
  NSFFile f(s);

  boost::lockfree::queue<int> q;

  PulseaudioDevice p(f.get_header());

  while (true) {

    vector<AudioData> v;

    for (int j = 0; j < 200; j++) {

      AudioData i = rand() % 1000;
      v.push_back(i);
    }

    const vector<AudioData> v2(v);
    p.write(v2);

    cout << "HELLO JS" << endl;
  }
}
