#ifndef DIXIEAUDIO_NSF_FILE_H
#define DIXIEAUDIO_NSF_FILE_H

#import "AudioFile.h"
#import <istream>
#import <vector>

enum NSFType { NTSC = 0, PAL = 1, DUAL = 2 };

enum ChipSupport {
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

struct NSFWord {
  using half_word = unsigned int;
  using value_type = unsigned int;

  half_word low;
  half_word high;

  NSFWord(half_word low, half_word high) : low{low}, high{high} {}
  NSFWord(){};

  value_type get_value() const { return (high << 8) + low; }
};

struct NSFHeader : public AudioHeader {
  using address = NSFWord;
  using speed = NSFWord;
  using quantity = NSFWord::half_word;
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
  NSFType NSF_type;
  flags extra_chip_support;
  std::vector<quantity> expansion_bits;

  NSFHeader(std::istream &file_stream);
  ~NSFHeader() override;

  unsigned int get_rate() const override;
};

class NSFFile : public AudioFile {
private:
  NSFHeader header;

public:
  NSFFile(std::istream &file_stream);
  std::vector<AudioData> read(size_t nb_bytes) override;
  const NSFHeader &get_header() const override;
  ~NSFFile() override;
};

std::ostream &operator<<(std::ostream &, const NSFWord &);

std::ostream &operator<<(std::ostream &, const NSFHeader &);

std::ostream &operator<<(std::ostream &, const NSFFile &);

#endif // DIXIEAUDIO_NSF_FILE_H
