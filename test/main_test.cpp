#include "gtest/gtest.h"

#include "AudioPlayerTests.cpp"
#include "NSFHeaderTests.cpp"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
