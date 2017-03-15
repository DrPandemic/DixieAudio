#ifndef DIXIEAUDIO_WAVFILE_H
#define DIXIEAUDIO_WAVFILE_H

#import "AudioFile.h"
#import <istream>
#import <vector>

class WAVFile : public AudioFile {};
struct WAVHeader : public AudioHeader {
  using string = std::string;
  using byte = uint8_t;
  using size = uint32_t;

  // 4 bytes
  size file_size;
  // 4 bytes
  // 4 bytes
  size format_data_length;
  uint16_t format_type;
  uint16_t number_of_channels;
  size sample_rate;
  // 4 bytes
  uint16_t channel_type;
  uint16_t bits_pet_sample;
  // 4 bytes
  size data_size;
};

#endif // DIXIEAUDIO_WAVFILE_H
