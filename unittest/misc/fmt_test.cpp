#include "fmt/color.h"
#include "fmt/format.h"
#include "fmt/ranges.h"  // IWYU pragma: keep
#include "gtest/gtest.h"

TEST(FMTTest, basic)
{
  std::vector<int> v = {1, 2, 3, 4, 5};
  std::string s      = fmt::format("{}", v);
  EXPECT_EQ(s, "[1, 2, 3, 4, 5]");
}

TEST(FMTTest, color)
{
  std::vector<int> v = {1, 2, 3, 4, 5};
  std::string s      = fmt::format(fg(fmt::color::red) | fmt::emphasis::bold, "Hello, {}!", v);
  fmt::println("{}", s);
  EXPECT_TRUE(true);
}

TEST(FMTTest, error)
{
  std::vector<int> v = {1, 2, 3, 4, 5};
  fmt::println(
      "{} {}",

      fmt::styled(
          "ERROR:",
          fmt::emphasis::bold | fmt::fg(fmt::color::red)
      ),

      fmt::styled(
          "missing semicolon",
          fmt::emphasis::italic
      )
  );
  EXPECT_TRUE(true);
}