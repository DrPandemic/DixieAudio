#include "../include/NSFFile.h"
#include <fstream>
#include <iostream>

using namespace std;

int main() {
  ifstream s("../nsf/mario.nsf", ifstream::in | std::ios::binary);
  NSFFile f(s);

  cout << f;
}
