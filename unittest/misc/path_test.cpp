#include <filesystem>

#include "gtest/gtest.h"

namespace fs = std::filesystem;

TEST(PathTest, cwd)
{
  fs::path cwd      = fs::current_path();
  fs::path unittest = cwd.filename();
  fs::path parent   = cwd.parent_path();
  fs::path build    = parent.filename();

  std::cout << "cwd: " << cwd << '\n';
  std::cout << "unittest: " << unittest << '\n';
  std::cout << "parent: " << parent << '\n';
  std::cout << "build: " << build << '\n';

  EXPECT_EQ(unittest, "unittest");
  EXPECT_EQ(build, "build");
}