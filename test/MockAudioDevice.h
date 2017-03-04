#include "AudioDevice.h"
#include "AudioFile.h"
#include "gmock/gmock.h"

class MockAudioDevice : public AudioDevice {
  MOCK_METHOD1(write, int(const std::vector<AudioData> data));
  MOCK_METHOD1(reset_device, void(const AudioHeader &header));
};
