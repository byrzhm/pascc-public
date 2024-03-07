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
  return parse(filename_);
}

}  // namespace pascc::parse