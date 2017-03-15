#include "WAVFile.cpp"
#include "gtest/gtest.h"
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>

TEST(reader_test, reader_test_WAVHeader) {
  unsigned char data[] = {0x52, 0x49, 0x46, 0x46, 0x94, 0x57, 0x03, 0x00, 0x57, 0x41, 0x56,
                          0x45, 0x66, 0x6D, 0x74, 0x20, 0x1E, 0x00, 0x00, 0x00, 0x55, 0x00,
                          0x01, 0x00, 0x11, 0x2B, 0x00, 0x00, 0xC4, 0x09, 0x00, 0x00, 0x01,
                          0x00, 0x00, 0x00, 0x0C, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00};

  std::stringstream ss(std::stringstream::in | std::stringstream::out);

  ss.write(reinterpret_cast<char const *>(data), sizeof data);

  WAVFile marioWAV(ss);
  WAVHeader marioHeader = marioWAV.get_header();

  EXPECT_EQ(marioHeader.file_size, 2488730368);
  EXPECT_EQ(marioHeader.format_data_length, 16);
  EXPECT_EQ(marioHeader.format_type, 1);
  EXPECT_EQ(marioHeader.number_of_channels, 2);
  EXPECT_EQ(marioHeader.sample_rate, 44100);
  EXPECT_EQ(marioHeader.channel_type, 4);
  EXPECT_EQ(marioHeader.bits_pet_sample, 16);
  EXPECT_EQ(marioHeader.data_size, 33554432);
}
