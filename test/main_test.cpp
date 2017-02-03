#include <limits.h>
#include <iostream>
#include "gtest/gtest.h"

using namespace std;

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  cout << "Hello yeh" << endl;
  return RUN_ALL_TESTS();
}
