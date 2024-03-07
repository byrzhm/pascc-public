#include "driver/driver.hpp"
#include "codegen/codegen.hpp"
#include "parse/parser_driver.hpp"

namespace pascc::driver {

auto Driver::Get() -> Driver &
{
  static Driver instance_;
  return instance_;
}

auto Driver::parse(const std::string &filepath) -> Driver &
{
  parse::ParserDriver drv(filepath, true, true);
  drv.parse();
  program_ = drv.program();
  return *this;
}

auto Driver::check() -> Driver &
{
  return *this;
}

auto Driver::codegen() -> Driver &
{
  codegen::CodegenVisitor visitor("test.c");
  program_->accept(visitor);
  return *this;
}

}  // namespace pascc::driver