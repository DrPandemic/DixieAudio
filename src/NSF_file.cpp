#include "../include/NSF_file.h"
#include "../include/audio_file.h"
#include <algorithm>
#include <iostream>
#include <iterator>

using namespace std;

NSF_header::NSF_header(istream &file_stream) {
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
    NSF_header::quantity tmp;
    copy_n(ii(file_stream), 1, &tmp);
    bankswitch_init_values.push_back(tmp);
  }
  copy_n(ii(file_stream), 1, &PAL_speed.low);
  copy_n(ii(file_stream), 1, &PAL_speed.high);

  int tmp;
  copy_n(ii(file_stream), 1, &tmp);
  if (tmp == NSF_type::NTSC)
    NSF_type = NSF_type::NTSC;
  else if (tmp == NSF_type::PAL)
    NSF_type = NSF_type::PAL;
  else
    NSF_type = NSF_type::DUAL;

  copy_n(ii(file_stream), 1, &extra_chip_support);
  expansion_bits.reserve(4);
  for (int i = 0; i < 4; ++i) {
    NSF_header::quantity tmp;
    copy_n(ii(file_stream), 1, &tmp);
    expansion_bits.push_back(tmp);
  }
}

NSF_file::NSF_file(std::istream &file_stream)
    : audio_file{file_stream}, header{file_stream} {}

NSF_file::~NSF_file() {}

vector<audio_data> NSF_file::read(size_t nb_bytes) {
  vector<audio_data> v;
  return v;
}

const NSF_header &NSF_file::getHeader() const { return header; }

std::ostream &operator<<(std::ostream &os, const NSF_word &w) {
  os << std::endl << "  Low: " << w.low << std::endl;
  return os << "  High: " << w.high;
}

std::ostream &operator<<(std::ostream &os, const NSF_header &h) {
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

std::ostream &operator<<(std::ostream &os, const NSF_file &f) {
  return os << f.getHeader();
}
