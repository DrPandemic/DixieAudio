#ifndef DIXIEAUDIO_WAVFILE_H
#define DIXIEAUDIO_WAVFILE_H

#import "AudioFile.h"
#import <istream>
#import <vector>

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
  uint16_t bits_per_sample;
  // 4 bytes
  size data_size;

  WAVHeader(std::istream &file_stream);
  ~WAVHeader() override{};

  unsigned int get_rate() const override;
  bool is_valid() const override {
    return format_type == 1 && number_of_channels == 1 && bits_per_sample == 16;
  }
};

class WAVFile : public AudioFile {
private:
  WAVHeader header;

public:
  WAVFile(std::istream &file_stream);
  std::vector<AudioData> read(size_t nb_bytes) override;
  const WAVHeader &get_header() const override;
  ~WAVFile() override;
};

std::ostream &operator<<(std::ostream &, const WAVHeader &);
std::ostream &operator<<(std::ostream &, const WAVFile &);

#endif // DIXIEAUDIO_WAVFILE_H
