#include <sstream>

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

struct Point
{
  double x_, y_;
  Point(double x, double y)
    : x_(x)
    , y_(y)
  {}
};

template<>
struct fmt::formatter<Point>: nested_formatter<double>
{
  auto format(Point p, format_context &ctx) const
  {
    return write_padded(ctx, [p, this](auto out) {
      return format_to(out, "({}, {})", nested(p.x_), nested(p.y_));
    });
  }
};


TEST(FMTTest, custom)
{
  Point p(1, 2);
  fmt::println("{}", p);
  EXPECT_TRUE(true);
}

auto operator<<(std::ostream &os, const Point &p) -> std::ostream &
{
  return os << "(" << p.x_ << ", " << p.y_ << ")";
}

TEST(FMTTest, sstream)
{
  Point p(1, 2);
  std::stringstream ss;
  ss << p;
  
  fmt::println(
      stderr,
      "this is {}!",
      fmt::styled(
          ss.str(),
          fmt::emphasis::bold | fmt::fg(fmt::color::green)
      )
  );
  EXPECT_TRUE(true);
}
