#ifndef DIXIEAUDIO_AUDIO_FILE_H
#define DIXIEAUDIO_AUDIO_FILE_H

#include <istream>
#include <memory>
#include <vector>

using AudioData = char;

struct AudioHeader {
  virtual unsigned int get_rate() const = 0;
  virtual ~AudioHeader(){};
  AudioHeader(){};
  virtual bool is_valid() const = 0;
};

class AudioFile {
protected:
  std::unique_ptr<std::istream> file_stream;

public:
  AudioFile(std::unique_ptr<std::istream> file_stream)
      : file_stream{std::move(file_stream)} {};
  virtual std::vector<AudioData> read(size_t nb_bytes) = 0;
  virtual std::vector<AudioData> read_all() = 0;
  virtual const AudioHeader &get_header() const = 0;
  virtual ~AudioFile(){};
  bool is_valid() const { return get_header().is_valid(); }
  bool eof() const { return file_stream->eof(); }
};

#endif // DIXIEAUDIO_AUDIO_FILE_H
