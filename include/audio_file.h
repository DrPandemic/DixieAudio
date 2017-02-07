//
// Created by bugz on 07/02/17.
//

#ifndef DIKIEAUDIO_AUDIO_FILE_H
#define DIKIEAUDIO_AUDIO_FILE_H

#include <istream>

struct audio_header {};

class audio_file {
public:
  virtual audio_file(std::istream &file_stream);
  virtual vector<audio_data> read(size_t nb_bytes);
  virtual audio_header getHeader() const;
};

#endif // DIKIEAUDIO_AUDIO_FILE_H
