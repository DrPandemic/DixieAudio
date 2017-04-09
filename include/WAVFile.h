#ifndef DIXIEAUDIO_WAVFILE_H
#define DIXIEAUDIO_WAVFILE_H

#include "AudioFile.h"
#include <istream>
#include <memory>
#include <vector>

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

  size_t get_header_size() const override { return 44; }
  size_t get_sample_size() const override { return bits_per_sample; }
};

class WAVFile : public AudioFile {
private:
  WAVHeader header;

public:
  WAVFile(std::unique_ptr<std::istream> file_stream);
  std::vector<AudioData> read(size_t nb_bytes) override;
  std::vector<AudioData> read_while(size_t nb_samples, us_t max_micro, bool add_lag = false) override;
  const WAVHeader &get_header() const override;
  ~WAVFile() override;
};

std::ostream &operator<<(std::ostream &, const WAVHeader &);
std::ostream &operator<<(std::ostream &, const WAVFile &);

#endif // DIXIEAUDIO_WAVFILE_H
