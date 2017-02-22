#include "NSFFile.cpp"
#include "gtest/gtest.h"
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>

TEST(reader_test, reader_test_NSFHeader) {

  unsigned char data[] = {
      0x4E, 0x45, 0x53, 0x4D, 0x1A, 0x01, 0x12, 0x01, 0xC4, 0xBD, 0x34, 0xBE,
      0xD0, 0xF2, 0x53, 0x75, 0x70, 0x65, 0x72, 0x20, 0x4D, 0x61, 0x72, 0x69,
      0x6F, 0x20, 0x42, 0x72, 0x6F, 0x73, 0x2E, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4B, 0x6F,
      0x6A, 0x69, 0x20, 0x4B, 0x6F, 0x6E, 0x64, 0x6F, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x39, 0x38, 0x35, 0x20, 0x4E,
      0x69, 0x6E, 0x74, 0x65, 0x6E, 0x64, 0x6F, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x1A, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  std::stringstream ss(std::stringstream::in | std::stringstream::out);

  ss.write(reinterpret_cast<char const *>(data), sizeof data);

  NSFFile marioNSF(ss);
  NSFHeader marioHeader = marioNSF.getHeader();

  std::vector<NSFHeader::quantity> bankswitch_init_values(8, 0);
  std::vector<NSFHeader::quantity> expansion_bits(4, 0);

  EXPECT_EQ(marioHeader.format_file, "NESM\x1A");
  EXPECT_EQ(marioHeader.version_number, 1);
  EXPECT_EQ(marioHeader.total_songs, 18);
  EXPECT_EQ(marioHeader.starting_song, 1);
  EXPECT_EQ(marioHeader.load_address.low, 0xC4);
  EXPECT_EQ(marioHeader.load_address.high, 0xBD);
  EXPECT_EQ(marioHeader.init_address.low, 0x34);
  EXPECT_EQ(marioHeader.init_address.high, 0xBE);
  EXPECT_EQ(marioHeader.play_address.low, 0xD0);
  EXPECT_EQ(marioHeader.play_address.high, 0xF2);
  EXPECT_EQ(marioHeader.song_name, "Super Mario Bros.");
  EXPECT_EQ(marioHeader.artist_name, "Koji Kondo");
  EXPECT_EQ(marioHeader.copyright_holder, "1985 Nintendo");
  EXPECT_EQ(marioHeader.NTSC_speed.low, 0x1A);
  EXPECT_EQ(marioHeader.NTSC_speed.high, 0x41);
  EXPECT_EQ(marioHeader.bankswitch_init_values, bankswitch_init_values);
  EXPECT_EQ(marioHeader.PAL_speed.low, 0);
  EXPECT_EQ(marioHeader.PAL_speed.high, 0);
  EXPECT_EQ(marioHeader.NSF_type, 0);
  EXPECT_EQ(marioHeader.extra_chip_support, NSFType::NTSC);
  EXPECT_EQ(marioHeader.expansion_bits, expansion_bits);
}

TEST(reader_test, reader_test_nsfword) {
  NSFWord word{26, 65};

  EXPECT_EQ(word.get_value(), 16666);
}
