#ifndef DIXIEAUDIO_AUDIO_FILE_H
#define DIXIEAUDIO_AUDIO_FILE_H

#include <chrono>
#include <istream>
#include <memory>
#include <vector>
#include <chrono>

using namespace std::chrono;

using AudioData = char;
using duration_t = system_clock::duration;

struct AudioHeader {
  virtual unsigned int get_rate() const = 0;
  virtual ~AudioHeader(){};
  AudioHeader(){};
  virtual bool is_valid() const = 0;
  virtual size_t get_header_size() const = 0;
};

class AudioFile {
protected:
  std::unique_ptr<std::istream> file_stream;

public:
  AudioFile(std::unique_ptr<std::istream> file_stream)
      : file_stream{std::move(file_stream)} {};
  virtual std::vector<AudioData> read(size_t nb_bytes) = 0;
  virtual std::vector<AudioData> read_while(size_t nb_samples,
                                            std::chrono::microseconds max_duration) = 0;
  virtual std::vector<AudioData> read_all() = 0;
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
