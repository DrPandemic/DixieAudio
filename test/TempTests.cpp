#include "gtest/gtest.h"

TEST(basic_test, basic_test_int) {
  int i = 3;
  int j = 3;

  EXPECT_EQ(i, 3);
  EXPECT_EQ(j, 3);
  EXPECT_EQ(i, j);
}

TEST(basic_test, basic_test_int_fail) {
  int i = 3;
  int j = 4;

  EXPECT_EQ(i, 3);
  EXPECT_EQ(j, 3);
  EXPECT_EQ(i, j);
}
//