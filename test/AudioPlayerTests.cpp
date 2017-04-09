#include "AudioPlayer.cpp"
#include "MockAudioDevice.h"
#include "NSFFile.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <cstdlib>
#include <memory>

using namespace std;
using namespace ::testing;

auto get_file() {

  unsigned char data[] = {0x52, 0x49, 0x46, 0x46, 0xC0, 0x27, 0x01, 0x00, 0x57,
                          0x41, 0x56, 0x45, 0x66, 0x6D, 0x74, 0x20, 0x10, 0x00,
                          0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x11, 0x2B, 0x00,
                          0x00, 0x22, 0x56, 0x00, 0x00, 0x02, 0x00, 0x10, 0x00,
                          0x64, 0x61, 0x74, 0x61, 0x9C, 0x27, 0x01, 0x00, 0x00};

  auto ss =
      make_unique<stringstream>(std::stringstream::in | std::stringstream::out);
  ss->write(reinterpret_cast<char const *>(data), sizeof data);

  auto f = make_unique<WAVFile>(move(ss));

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

TEST(player_test, timing) {
  // player instantiation
  auto device_ptr = new StrictMock<MockAudioDevice>{};
  EXPECT_CALL(*device_ptr, reset_device(_)).WillOnce(Return());
  EXPECT_CALL(*device_ptr, write(_)).WillRepeatedly(Return(1));
  auto device = unique_ptr<StrictMock<MockAudioDevice>>(device_ptr);
  AudioPlayer player{move(device)};
  auto file = get_file();

  player.start(std::move(file));
  ::sleep(1);
  AudioPlayerTimingInfo t = player.get_timing_info();
  auto sample_played_estimation =
      t.time_elapsed_since_first_write.count() * t.sample_rate_us;

  auto diff_written_sample =
      abs(sample_played_estimation - t.current_sample_written);
  auto wanted_buffer_size = AudioPlayer::BUFFER_US.count() * t.sample_rate_us;

  ASSERT_LE(diff_written_sample, wanted_buffer_size + 8);

  player.kill();
}
