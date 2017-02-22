#ifndef DIXIEAUDIO_AUDIO_FILE_H
#define DIXIEAUDIO_AUDIO_FILE_H

#include <istream>
#include <vector>

using audio_data = char;

struct audio_header {};

class AudioFile {
protected:
  std::istream &file_stream;

public:
  AudioFile(std::istream &file_stream) : file_stream{file_stream} {};
  virtual std::vector<audio_data> read(size_t nb_bytes) = 0;
  virtual const audio_header &getHeader() const = 0;
  virtual ~AudioFile(){};
};

#endif // DIXIEAUDIO_AUDIO_FILE_H
