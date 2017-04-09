#include "WAVFile.cpp"
#include "gtest/gtest.h"
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <boost/chrono.hpp>

TEST(reader_test, get_header) {

  unsigned char data[] = {0x52, 0x49, 0x46, 0x46, 0xC0, 0x27, 0x01, 0x00, 0x57,
                          0x41, 0x56, 0x45, 0x66, 0x6D, 0x74, 0x20, 0x10, 0x00,
                          0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x11, 0x2B, 0x00,
                          0x00, 0x22, 0x56, 0x00, 0x00, 0x02, 0x00, 0x10, 0x00,
                          0x64, 0x61, 0x74, 0x61, 0x9C, 0x27, 0x01, 0x00};

  auto ss =
      make_unique<stringstream>(std::stringstream::in | std::stringstream::out);

  ss->write(reinterpret_cast<char const *>(data), sizeof data);

  WAVFile marioWAV(move(ss));
  WAVHeader marioHeader = marioWAV.get_header();

  EXPECT_EQ(marioHeader.file_size, 75712);
  EXPECT_EQ(marioHeader.format_data_length, 16);
  EXPECT_EQ(marioHeader.format_type, 1);
  EXPECT_EQ(marioHeader.number_of_channels, 1);
  EXPECT_EQ(marioHeader.sample_rate, 11025);
  EXPECT_EQ(marioHeader.channel_type, 2);
  EXPECT_EQ(marioHeader.bits_per_sample, 16);
  EXPECT_EQ(marioHeader.data_size, 75676);
}

TEST(reader_test, is_valid) {

  unsigned char not_valid_data[] = {
      0x52, 0x49, 0x46, 0x46, 0x94, 0x57, 0x03, 0x00, 0x57, 0x41, 0x56,
      0x45, 0x66, 0x6D, 0x74, 0x20, 0x1E, 0x00, 0x00, 0x00, 0x55, 0x00,
      0x01, 0x00, 0x11, 0x2B, 0x00, 0x00, 0xC4, 0x09, 0x00, 0x00, 0x01,
      0x00, 0x00, 0x00, 0x0C, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00};

  unsigned char valid_data[] = {
      0x52, 0x49, 0x46, 0x46, 0xC0, 0x27, 0x01, 0x00, 0x57, 0x41, 0x56,
      0x45, 0x66, 0x6D, 0x74, 0x20, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00,
      0x01, 0x00, 0x11, 0x2B, 0x00, 0x00, 0x22, 0x56, 0x00, 0x00, 0x02,
      0x00, 0x10, 0x00, 0x64, 0x61, 0x74, 0x61, 0x9C, 0x27, 0x01, 0x00};

  auto ss_v =
      make_unique<stringstream>(std::stringstream::in | std::stringstream::out);
  auto ss_not_v =
      make_unique<stringstream>(std::stringstream::in | std::stringstream::out);

  ss_v->write(reinterpret_cast<char const *>(valid_data), sizeof valid_data);
  ss_not_v->write(reinterpret_cast<char const *>(not_valid_data),
                  sizeof not_valid_data);

  WAVFile marioWAV_v(move(ss_v));
  WAVFile marioWAV_not_v(move(ss_not_v));

  EXPECT_EQ(marioWAV_v.is_valid(), true);
  EXPECT_EQ(marioWAV_not_v.is_valid(), false);
}

TEST(reader_test, read) {

  unsigned char valid_data[] = {
      0x52, 0x49, 0x46, 0x46, 0xC0, 0x27, 0x01, 0x00, 0x57, 0x41, 0x56,
      0x45, 0x66, 0x6D, 0x74, 0x20, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00,
      0x01, 0x00, 0x11, 0x2B, 0x00, 0x00, 0x22, 0x56, 0x00, 0x00, 0x02,
      0x00, 0x10, 0x00, 0x64, 0x61, 0x74, 0x61, 0x9C, 0x27, 0x01, 0x00,
      0x01,0x65,0x03,0x04};

  auto ss_v =
      make_unique<stringstream>(std::stringstream::in | std::stringstream::out);

  ss_v->write(reinterpret_cast<char const *>(valid_data), sizeof valid_data);

  WAVFile marioWAV_v(move(ss_v));

  vector<AudioData> data = marioWAV_v.read(4);

  EXPECT_EQ(data.size(), 4);
  EXPECT_EQ(data[0], 1);
  EXPECT_EQ(data[1], 101);
  EXPECT_EQ(data[2], 3);
  EXPECT_EQ(data[3], 4);
}

TEST(reader_test, read_while) {

  unsigned char valid_data[] = {
      0x52, 0x49, 0x46, 0x46, 0xC0, 0x27, 0x01, 0x00, 0x57, 0x41, 0x56,
      0x45, 0x66, 0x6D, 0x74, 0x20, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00,
      0x01, 0x00, 0x11, 0x2B, 0x00, 0x00, 0x22, 0x56, 0x00, 0x00, 0x02,
      0x00, 0x10, 0x00, 0x64, 0x61, 0x74, 0x61, 0x9C, 0x27, 0x01, 0x00,
      0x01, 0x65, 0x03, 0x04, 0x01, 0x65, 0x03, 0x04};

  auto ss_v =
      make_unique<stringstream>(std::stringstream::in | std::stringstream::out);

  ss_v->write(reinterpret_cast<char const *>(valid_data), sizeof valid_data);

  WAVFile marioWAV_v(move(ss_v));
  auto header = marioWAV_v.get_header();

  auto before = boost::chrono::system_clock::now();
  vector<AudioData> data = marioWAV_v.read_while(4, us_t(200));
  auto after = boost::chrono::system_clock::now();

  EXPECT_EQ(data.size(), 4 * (header.bits_per_sample / (sizeof(AudioData) * 8)));
  EXPECT_EQ(data[0], 1);
  EXPECT_EQ(data[1], 101);
  EXPECT_EQ(data[2], 3);
  EXPECT_EQ(data[3], 4);
  EXPECT_LE(boost::chrono::duration_cast<us_t>(after-before), us_t(200));

  before = boost::chrono::system_clock::now();
  data = marioWAV_v.read_while(4, us_t(70), true);
  after = boost::chrono::system_clock::now();

  cout << boost::chrono::duration_cast<us_t>(after-before) << endl;
  cout << us_t(header.bits_per_sample/8*30)<<endl;

  EXPECT_LE(data.size(), 4);
  EXPECT_LE(boost::chrono::duration_cast<us_t>(after-before), us_t(70) + us_t(header.bits_per_sample/8*30));
}


