#include "driver/driver.hpp"

using pascc::driver::Driver;

auto main() -> int
{
  Driver::Get()
      .parse("test.pas")
      .check()
      .codegen();
  return 0;
}