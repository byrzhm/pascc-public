#pragma once

#include "location.hh"
#include "parser.hpp"

#define YY_DECL auto yylex(pascc::parse::ParserDriver &drv) -> pascc::parse::Parser::symbol_type
YY_DECL;

namespace pascc::parse {

class ParserDriver
{
  friend class Parser;

public:
  ParserDriver() = default;

  ParserDriver(std::string filename, bool trace_scanning, bool trace_parsing)
    : filename_(std::move(filename))
    , trace_scanning_(trace_scanning)
    , trace_parsing_(trace_parsing)
  {
  }

  auto location() -> location & { return loc_; }

  void scan_begin();

  void scan_end();

  auto parse(const std::string &filename) -> int;

  auto parse() -> int;

  auto program() -> std::unique_ptr<Program> { return std::move(program_); }

private:
  class location loc_;
  std::string filename_;
  bool trace_scanning_ = false;
  bool trace_parsing_  = false;

  std::unique_ptr<Program> program_;
};

}  // namespace pascc::parse
