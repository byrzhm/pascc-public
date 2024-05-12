#include "driver/driver.hpp"
#include "codegen/codegen.hpp"
#include "parse/parser_driver.hpp"
#include "semant/semant.hpp"
#include "util/log.hpp"

namespace pascc::driver {

auto Driver::get() -> Driver &
{
  static Driver instance_;
  return instance_;
}

auto Driver::parse(const std::string &filepath) -> Driver &
{
  parse::ParserDriver drv(&filepath, false, false);
  int result = drv.parse();
  if (result != 0) {
    LOG_ERROR("{}", "Parsing failed");
    exit(EXIT_FAILURE);
  }
  program_ = drv.program();
  return *this;
}

auto Driver::check() -> Driver &
{
  semant::SemantVisitor visitor;
  bool success = true;
  try {
    program_->accept(visitor);
  } catch (const std::exception &e) {
    success = false;
  }
  if (!success || !visitor.error_msgs().empty()) {
    for (const auto &msg : visitor.error_msgs()) {
      LOG_ERROR("{}", msg);
    }
    exit(EXIT_FAILURE);
  }
  return *this;
}

auto Driver::codegen(const std::string &filepath) -> Driver &
{
  codegen::CodegenVisitor visitor(filepath);
  program_->accept(visitor);
  return *this;
}

}  // namespace pascc::driver
