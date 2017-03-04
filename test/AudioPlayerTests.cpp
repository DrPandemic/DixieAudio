#include "AudioPlayer.h"
#include "MockAudioDevice.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <memory>

using namespace std;
using namespace ::testing;

TEST(player_test, can_start_playing) {
  auto audio_device = make_unique<StrictMock<MockAudioDevice>>();
}
