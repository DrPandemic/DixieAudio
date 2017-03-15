#include "../include/WAVFile.h"

#include <algorithm>
#include <iostream>
#include <iterator>

using namespace std;

WAVHeader::WAVHeader(istream &file_stream) : AudioHeader() {
  file_stream >> std::noskipws;
  using ii = istream_iterator<uint8_t>;
  uint32_t garbage;

  copy_n(ii(file_stream), 4, &garbage);
  copy_n(ii(file_stream), 4, &file_size);
  copy_n(ii(file_stream), 4, &garbage);
  copy_n(ii(file_stream), 4, &garbage);
  copy_n(ii(file_stream), 4, &format_data_length);
  copy_n(ii(file_stream), 2, &format_type);
  copy_n(ii(file_stream), 2, &number_of_channels);
  copy_n(ii(file_stream), 4, &sample_rate);
  copy_n(ii(file_stream), 4, &garbage);
  copy_n(ii(file_stream), 2, &channel_type);
  copy_n(ii(file_stream), 2, &bits_per_sample);
  copy_n(ii(file_stream), 4, &garbage);
  copy_n(ii(file_stream), 4, &data_size);
}

unsigned int WAVHeader::get_rate() const { return sample_rate; }

WAVFile::WAVFile(std::istream &file_stream)
    : AudioFile{file_stream}, header{file_stream} {}

WAVFile::~WAVFile() {}

vector<AudioData> WAVFile::read(size_t nb_bytes) {
  vector<AudioData> v;
  return v;
}

const WAVHeader &WAVFile::get_header() const { return header; }

std::ostream &operator<<(std::ostream &os, const WAVHeader &h) {
  os << "File size: " << h.file_size << std::endl;
  os << "Format data length: " << h.format_data_length << std::endl;
  os << "Format type: " << h.format_type << std::endl;
  os << "Number of channels: " << h.number_of_channels << std::endl;
  os << "Sample rate: " << h.sample_rate << std::endl;
  os << "Channel type: " << h.channel_type << std::endl;
  os << "Bits per sample: " << h.bits_per_sample << std::endl;
  os << "Data size: " << h.data_size << std::endl;

  return os;
}

std::ostream &operator<<(std::ostream &os, const WAVFile &f) {
  return os << f.get_header();
}
