#include "driver/driver.hpp"
#include "codegen/codegen.hpp"
#include "parse/parser_driver.hpp"
#include "semant/semant.hpp"

namespace pascc::driver {

auto Driver::Get() -> Driver &
{
  static Driver instance_;
  return instance_;
}

auto Driver::parse(const std::string &filepath) -> Driver &
{
  parse::ParserDriver drv(filepath, false, false);
  drv.parse();
  program_ = drv.program();
  return *this;
}

auto Driver::check() -> Driver &
{
  semant::SemantVisitor visitor;
  program_->accept(visitor);
  return *this;
}

auto Driver::codegen(const std::string &filepath) -> Driver &
{
  codegen::CodegenVisitor visitor(filepath);
  program_->accept(visitor);
  return *this;
}

}  // namespace pascc::driver