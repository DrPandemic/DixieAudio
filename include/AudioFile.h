#ifndef DIXIEAUDIO_AUDIO_FILE_H
#define DIXIEAUDIO_AUDIO_FILE_H

#include <boost/chrono.hpp>
#include <istream>
#include <memory>
#include <vector>

using AudioData = char;
using duration_t = boost::chrono::system_clock::duration;
using time_point_t = boost::chrono::system_clock::time_point;
using us_t = boost::chrono::microseconds;

struct AudioHeader {
  virtual unsigned int get_rate() const = 0;
  virtual ~AudioHeader(){};
  AudioHeader(){};
  virtual bool is_valid() const = 0;
  virtual size_t get_header_size() const = 0;
  virtual size_t get_sample_size() const = 0;
};

class AudioFile {
protected:
  std::unique_ptr<std::istream> file_stream;

public:
  AudioFile(std::unique_ptr<std::istream> file_stream)
      : file_stream{std::move(file_stream)} {};
  virtual std::vector<AudioData> read(size_t nb_bytes) = 0;
  virtual std::vector<AudioData> read_while(size_t nb_samples,
                                            us_t max_duration, bool add_lag = false) = 0;
  virtual const AudioHeader &get_header() const = 0;
  virtual ~AudioFile(){};
  bool is_valid() const { return get_header().is_valid(); }
  bool eof() const { return file_stream->eof(); }
  void restart() {
    file_stream->clear();
    file_stream->seekg(get_header().get_header_size(), std::ios::beg);
  }
  std::streampos tellg() const { return file_stream->tellg(); }
};

#endif // DIXIEAUDIO_AUDIO_FILE_H
