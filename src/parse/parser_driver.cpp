#include "parse/parser_driver.hpp"
#include "parser.hpp"

namespace pascc::parse {

auto ParserDriver::parse(const std::string &filename) -> int
{
  loc_.initialize(&filename);
  scan_begin();
  Parser parser(*this);
  parser.set_debug_level(static_cast<Parser::debug_level_type>(trace_parsing_));
  const auto result = parser.parse();
  scan_end();
  return result;
}

auto ParserDriver::parse() -> int
{
  return parse(*filename_);
}

void ParserDriver::addFunction(std::string funcid)
{
  funcid_set_.insert(std::move(funcid));
}

auto ParserDriver::isFunction(const std::string &funcid) -> bool
{
  return funcid_set_.find(funcid) != funcid_set_.end();
}

void ParserDriver::pushCurrentFunction(const std::string &funcid)
{
  function_stack_.push(funcid);
}

void ParserDriver::popCurrentFunction()
{
  function_stack_.pop();
}

auto ParserDriver::currentFunction() -> const std::string &
{
  return function_stack_.top();
}

}  // namespace pascc::parse