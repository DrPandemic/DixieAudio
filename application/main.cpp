#include "../include/NSF_file.h"
#include <fstream>
#include <iostream>

using namespace std;

int main() {
  ifstream s("../nsf/mario.nsf", ifstream::in | std::ios::binary);
  NSF_file f(s);

  cout << f;
}
