#include "../include/NSFFile.h"
#include "../include/AudioFile.h"
#include <algorithm>
#include <iostream>
#include <iterator>

using namespace std;

NSFHeader::NSFHeader(istream &file_stream) : AudioHeader() {
  file_stream >> std::noskipws;
  using ii = istream_iterator<uint8_t>;

  copy_n(ii(file_stream), 5, back_inserter(format_file));
  copy_n(ii(file_stream), 1, &version_number);
  copy_n(ii(file_stream), 1, &total_songs);
  copy_n(ii(file_stream), 1, &starting_song);
  copy_n(ii(file_stream), 1, &load_address.low);
  copy_n(ii(file_stream), 1, &load_address.high);
  copy_n(ii(file_stream), 1, &init_address.low);
  copy_n(ii(file_stream), 1, &init_address.high);
  copy_n(ii(file_stream), 1, &play_address.low);
  copy_n(ii(file_stream), 1, &play_address.high);
  copy_n(ii(file_stream), 32, back_inserter(song_name));
  song_name = string{song_name.c_str()};
  copy_n(ii(file_stream), 32, back_inserter(artist_name));
  artist_name = string{artist_name.c_str()};
  copy_n(ii(file_stream), 32, back_inserter(copyright_holder));
  copyright_holder = string{copyright_holder.c_str()};
  copy_n(ii(file_stream), 1, &NTSC_speed.low);
  copy_n(ii(file_stream), 1, &NTSC_speed.high);
  bankswitch_init_values.reserve(8);
  for (int i = 0; i < 8; ++i) {
    NSFHeader::quantity tmp;
    copy_n(ii(file_stream), 1, &tmp);
    bankswitch_init_values.push_back(tmp);
  }
  copy_n(ii(file_stream), 1, &PAL_speed.low);
  copy_n(ii(file_stream), 1, &PAL_speed.high);

  int tmp;
  copy_n(ii(file_stream), 1, &tmp);
  if (tmp == NSFType::NTSC)
    NSF_type = NSFType::NTSC;
  else if (tmp == NSFType::PAL)
    NSF_type = NSFType::PAL;
  else
    NSF_type = NSFType::DUAL;

  copy_n(ii(file_stream), 1, &extra_chip_support);
  expansion_bits.reserve(4);
  for (int i = 0; i < 4; ++i) {
    NSFHeader::quantity tmp;
    copy_n(ii(file_stream), 1, &tmp);
    expansion_bits.push_back(tmp);
  }
}

unsigned int NSFHeader::get_rate() const {
  if (NSF_type == NSFType::PAL) {
    return PAL_speed.get_value();
  } else {
    return NTSC_speed.get_value();
  }
}

NSFHeader::~NSFHeader() {}

NSFFile::NSFFile(std::istream &file_stream)
    : AudioFile{file_stream}, header{file_stream} {}

NSFFile::~NSFFile() {}

vector<AudioData> NSFFile::read(size_t nb_bytes) {
  vector<AudioData> v;
  return v;
}

const NSFHeader &NSFFile::get_header() const { return header; }

std::ostream &operator<<(std::ostream &os, const NSFWord &w) {
  os << std::endl << "  Low: " << w.low << std::endl;
  return os << "  High: " << w.high;
}

std::ostream &operator<<(std::ostream &os, const NSFHeader &h) {
  os << "Format: " << h.format_file << std::endl;
  os << "Version: " << h.version_number << std::endl;
  os << "Songs: " << h.total_songs << std::endl;
  os << "Starting: " << h.starting_song << std::endl;
  os << "Load: " << h.load_address << std::endl;
  os << "Init: " << h.init_address << std::endl;
  os << "Play:  " << h.play_address << std::endl;
  os << "Name: " << h.song_name << std::endl;
  os << "Artist: " << h.artist_name << std::endl;
  os << "Copyright: " << h.copyright_holder << std::endl;
  os << "NTSC speed: " << h.NTSC_speed << std::endl;
  os << "bankswitch init value: " << std::endl;
  for (int i = 0; i < 8; ++i)
    os << "  " << h.bankswitch_init_values[i] << std::endl;
  os << "PAL speed: " << h.PAL_speed << std::endl;
  os << "Type: " << h.NSF_type << std::endl;
  os << "Extra chip shupport: " << h.extra_chip_support << std::endl;
  os << "Expension bits: " << std::endl;
  for (int i = 0; i < 4; ++i)
    os << "  " << h.expansion_bits[i] << std::endl;

  return os;
}

std::ostream &operator<<(std::ostream &os, const NSFFile &f) {
  return os << f.get_header();
}
