#include "AudioPlayer.cpp"
#include "MockAudioDevice.h"
#include "NSFFile.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <memory>

using namespace std;
using namespace ::testing;

auto get_file() {
  unsigned char data[] = {
      0x4E, 0x45, 0x53, 0x4D, 0x1A, 0x01, 0x12, 0x01, 0xC4, 0xBD, 0x34, 0xBE,
      0xD0, 0xF2, 0x53, 0x75, 0x70, 0x65, 0x72, 0x20, 0x4D, 0x61, 0x72, 0x69,
      0x6F, 0x20, 0x42, 0x72, 0x6F, 0x73, 0x2E, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4B, 0x6F,
      0x6A, 0x69, 0x20, 0x4B, 0x6F, 0x6E, 0x64, 0x6F, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x39, 0x38, 0x35, 0x20, 0x4E,
      0x69, 0x6E, 0x74, 0x65, 0x6E, 0x64, 0x6F, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x1A, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  auto ss =
      make_unique<stringstream>(std::stringstream::in | std::stringstream::out);
  ss->write(reinterpret_cast<char const *>(data), sizeof data);
  auto f = make_unique<NSFFile>(move(ss));

  return f;
}

TEST(player_test, can_start_playing) {
  auto device_ptr = new StrictMock<MockAudioDevice>{};
  EXPECT_CALL(*device_ptr, reset_device(_)).WillOnce(Return());
  EXPECT_CALL(*device_ptr, write(_)).WillRepeatedly(Return(1));
  auto device = unique_ptr<StrictMock<MockAudioDevice>>(device_ptr);
  AudioPlayer player{move(device)};
  auto file = get_file();

  ASSERT_EQ(player.get_state(), AudioPlayerState::stopped);
  player.start(std::move(file));
  ASSERT_EQ(player.get_state(), AudioPlayerState::playing);

  player.kill();
}

TEST(player_test, can_resume) {
  auto device_ptr = new StrictMock<MockAudioDevice>{};
  EXPECT_CALL(*device_ptr, reset_device(_)).WillOnce(Return());
  EXPECT_CALL(*device_ptr, write(_)).WillRepeatedly(Return(1));
  auto device = unique_ptr<StrictMock<MockAudioDevice>>(device_ptr);
  AudioPlayer player{move(device)};
  auto file = get_file();

  ASSERT_EQ(player.get_state(), AudioPlayerState::stopped);
  player.start(std::move(file));
  ASSERT_EQ(player.get_state(), AudioPlayerState::playing);
  player.stop();
  ASSERT_EQ(player.get_state(), AudioPlayerState::paused);
  player.resume();
  ASSERT_EQ(player.get_state(), AudioPlayerState::playing);

  player.kill();
}
