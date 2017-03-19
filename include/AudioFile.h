#ifndef DIXIEAUDIO_AUDIO_FILE_H
#define DIXIEAUDIO_AUDIO_FILE_H

#include <istream>
#include <vector>

using AudioData = unsigned int;

struct AudioHeader {
  virtual unsigned int get_rate() const = 0;
  virtual ~AudioHeader(){};
  AudioHeader(){};
  virtual bool is_valid() const = 0;
};

class AudioFile {
protected:
  std::istream &file_stream;

public:
  AudioFile(std::istream &file_stream) : file_stream{file_stream} {};
  virtual std::vector<AudioData> read(size_t nb_bytes) = 0;
  virtual const AudioHeader &get_header() const = 0;
  virtual ~AudioFile(){};
  bool is_valid() { return get_header().is_valid(); }
};

#endif // DIXIEAUDIO_AUDIO_FILE_H
