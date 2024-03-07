#include "driver/driver.hpp"

namespace pascc::driver {

auto Driver::Get() -> Driver &
{
  static Driver instance_;
  return instance_;
}

auto Driver::parse([[maybe_unused]] const std::string &filepath) -> Driver &
{
  return *this;
}

auto Driver::check() -> Driver &
{
  return *this;
}

auto Driver::codegen() -> Driver &
{
  return *this;
}

}  // namespace pascc::driver