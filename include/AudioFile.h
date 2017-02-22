#ifndef DIXIEAUDIO_AUDIO_FILE_H
#define DIXIEAUDIO_AUDIO_FILE_H

#include <istream>
#include <vector>

using AudioData = char;

struct AudioHeader {
  virtual unsigned int get_rate() const = 0;
  virtual ~AudioHeader(){};
  AudioHeader(){};
};

class AudioFile {
protected:
  std::istream &file_stream;

public:
  AudioFile(std::istream &file_stream) : file_stream{file_stream} {};
  virtual std::vector<AudioData> read(size_t nb_bytes) = 0;
  virtual const AudioHeader &getHeader() const = 0;
  virtual ~AudioFile(){};
};

#endif // DIXIEAUDIO_AUDIO_FILE_H
