#ifndef DIXIEAUDIO_NSF_FILE_H
#define DIXIEAUDIO_NSF_FILE_H

#import "./audio_file.h"
#import <istream>
#import <vector>

enum NSF_type { NTSC = 0, PAL = 1, DUAL = 2 };

enum chip_support {
  NONE = 0,
  VRCVI = 1 << 0,
  VCRVII = 1 << 1,
  FDS = 1 << 2,
  MMC5 = 1 << 3,
  Namco_106 = 1 << 4,
  Sunsoft_FME_07 = 1 << 5,
  expansion_1 = 1 << 6,
  expansion_2 = 1 << 7
};

struct NSF_word {
  using half_word = unsigned int;

  half_word low;
  half_word high;

  NSF_word(half_word low, half_word high) : low{low}, high{high} {}
  NSF_word(){};
};

struct NSF_header : public audio_header {
  using address = NSF_word;
  using speed = NSF_word;
  using quantity = NSF_word::half_word;
  using flags = quantity;

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
  speed NTSC_speed;
  std::vector<quantity> bankswitch_init_values;
  speed PAL_speed;
  NSF_type NSF_type;
  flags extra_chip_support;
  std::vector<quantity> expansion_bits;

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

std::ostream &operator<<(std::ostream &, const NSF_word &);

std::ostream &operator<<(std::ostream &, const NSF_header &);

std::ostream &operator<<(std::ostream &, const NSF_file &);

#endif // DIXIEAUDIO_NSF_FILE_H
