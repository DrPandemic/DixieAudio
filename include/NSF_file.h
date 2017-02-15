//
// Created by bugz on 07/02/17.
//
#ifndef DIXIEAUDIO_NSF_FILE_H
#define DIXIEAUDIO_NSF_FILE_H

#import "./audio_file.h"
#import <istream>
#import <vector>

enum NSF_type { PAL, NTSC, DUAL };

enum chip_support {
  VRCVI = 1 << 0,
  VCRVII = 1 << 1,
  FDS = 1 << 2,
  MMC5 = 1 << 3,
  Namco_106 = 1 << 4,
  Sunsoft_FME_07 = 1 << 5,
  expansion_1 = 1 << 6,
  expansion_2 = 1 << 7
};

struct nsf_word {
  using address = unsigned int;

  address low;
  address high;

  nsf_word(address low, address high) : low{low}, high{high} {}
  nsf_word(){};
};

struct NSF_header : public audio_header {
  using address = nsf_word;
  using speed = nsf_word;
  using quantity = unsigned int;

  std::string format_file;
  quantity version_number;
  quantity total_songs;
  quantity starting_song;
  address load_address;
  address init_address;
  address play_address;
  std::string song_name;
  std::string artist_name;
  std::string copyright_holder;
  quantity NTSC_speed;
  quantity bankswitch_init_values;
  quantity PAL_speed;
  NSF_type type;
  quantity extra_chip_support;
  quantity expansion_bits;

  NSF_header(std::istream &file_stream);
};

class NSF_file : public audio_file {
private:
  NSF_header header;

public:
  NSF_file(std::istream &file_stream);
  std::vector<audio_data> read(size_t nb_bytes) override;
  const NSF_header &getHeader() const override;
  ~NSF_file() override;
};

#endif // DIXIEAUDIO_NSF_FILE_H
