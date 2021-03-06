#include "../include/WAVFile.h"
#include <boost/thread.hpp>

using namespace std;
using namespace boost::chrono;

template <class destination> void read(istream &fs, destination &dest) {
  fs.read(reinterpret_cast<char *>(&dest), sizeof(dest));
}

WAVHeader::WAVHeader(istream &file_stream) : AudioHeader() {
  file_stream >> std::noskipws;
  uint32_t garbage;

  read(file_stream, garbage);
  read(file_stream, file_size);
  read(file_stream, garbage);
  read(file_stream, garbage);
  read(file_stream, format_data_length);
  read(file_stream, format_type);
  read(file_stream, number_of_channels);
  read(file_stream, sample_rate);
  read(file_stream, garbage);
  read(file_stream, channel_type);
  read(file_stream, bits_per_sample);
  read(file_stream, garbage);
  read(file_stream, data_size);
}

unsigned int WAVHeader::get_rate() const { return sample_rate; }

WAVFile::WAVFile(unique_ptr<istream> file_stream)
    : AudioFile{move(file_stream)}, header{*this->file_stream} {}

WAVFile::~WAVFile() {}

vector<AudioData> WAVFile::read(size_t nb_bytes) {
  vector<AudioData> data;
  data.reserve(nb_bytes);
  char tmp;
  for (size_t i = 0; file_stream->good() && i < nb_bytes; ++i) {
    file_stream->read(&tmp, 1);
    data.push_back(tmp);
  }

  return data;
}

vector<AudioData> WAVFile::read_while(size_t nb_samples, us_t max_micro, bool add_lag) {
  auto start = system_clock::now();
  vector<AudioData> data;
  int amount = header.bits_per_sample / (sizeof(AudioData) * 8);
  data.reserve(nb_samples * amount);
  AudioData tmp;
  for (size_t i = 0;
       file_stream->good() && i < nb_samples &&
       duration_cast<us_t>(system_clock::now() - start) < max_micro;
       ++i) {
    for (int j = 0; j < amount && file_stream->good(); ++j) {
      if(add_lag) {
        boost::this_thread::sleep_for(
            us_t(30));
      }
      file_stream->read(&tmp, 1);
      data.push_back(tmp);
    }
  }

  return data;
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
