#include "util/log.hpp"
#include "gtest/gtest.h"

TEST(LogTest, error)
{
  LOG_ERROR("{}", "missing semicolon");
  EXPECT_TRUE(true);
}