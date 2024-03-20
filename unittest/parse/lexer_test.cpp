#include <fstream>

#include "parse/parser_driver.hpp"
#include "parser.hpp"
#include "gtest/gtest.h"

using pascc::parse::Parser;

auto symbol_to_string(const Parser::symbol_type &symbol) -> std::string
{
  switch (symbol.kind_) {
    // token
    case pascc::parse::Parser::symbol_kind::S_PROGRAM: return "program";
    case pascc::parse::Parser::symbol_kind::S_BEGIN: return "begin";
    case pascc::parse::Parser::symbol_kind::S_END: return "end";
    case pascc::parse::Parser::symbol_kind::S_WRITELN: return "writeln";
    case pascc::parse::Parser::symbol_kind::S_SEMICOLON: return ";";
    case pascc::parse::Parser::symbol_kind::S_PERIOD: return ".";
    case pascc::parse::Parser::symbol_kind::S_LPAREN: return "(";
    case pascc::parse::Parser::symbol_kind::S_RPAREN: return ")";
    case pascc::parse::Parser::symbol_kind::S_ID: return symbol.value.as<std::string>();
    case pascc::parse::Parser::symbol_kind::S_STR_LIT: return symbol.value.as<std::string>();

    // non-terminal
    case pascc::parse::Parser::symbol_kind::S_program:
    case pascc::parse::Parser::symbol_kind::S_program_head:
    case pascc::parse::Parser::symbol_kind::S_program_body:
    case pascc::parse::Parser::symbol_kind::S_statement_block:
    case pascc::parse::Parser::symbol_kind::S_statement:
    case pascc::parse::Parser::symbol_kind::S_procedure_statement: return "nonterminal";

    case pascc::parse::Parser::symbol_kind::YYNTOKENS:
    case pascc::parse::Parser::symbol_kind::S_YYEMPTY:
    case pascc::parse::Parser::symbol_kind::S_YYEOF:
    case pascc::parse::Parser::symbol_kind::S_YYerror:
    case pascc::parse::Parser::symbol_kind::S_YYUNDEF: return "Unknown";
  }
}

auto symbol_to_string(const Parser::symbol_kind_type &kind, const std::string &id = "identifier") -> std::string
{
  switch (kind) {
    // token
    case pascc::parse::Parser::symbol_kind::S_PROGRAM: return "program";
    case pascc::parse::Parser::symbol_kind::S_BEGIN: return "begin";
    case pascc::parse::Parser::symbol_kind::S_END: return "end";
    case pascc::parse::Parser::symbol_kind::S_WRITELN: return "writeln";
    case pascc::parse::Parser::symbol_kind::S_SEMICOLON: return ";";
    case pascc::parse::Parser::symbol_kind::S_PERIOD: return ".";
    case pascc::parse::Parser::symbol_kind::S_LPAREN: return "(";
    case pascc::parse::Parser::symbol_kind::S_RPAREN: return ")";
    case pascc::parse::Parser::symbol_kind::S_ID: return id;
    case pascc::parse::Parser::symbol_kind::S_STR_LIT: return "'string literal'";

    // non-terminal
    case pascc::parse::Parser::symbol_kind::S_program:
    case pascc::parse::Parser::symbol_kind::S_program_head:
    case pascc::parse::Parser::symbol_kind::S_program_body:
    case pascc::parse::Parser::symbol_kind::S_statement_block:
    case pascc::parse::Parser::symbol_kind::S_statement:
    case pascc::parse::Parser::symbol_kind::S_procedure_statement: return "nonterminal";

    case pascc::parse::Parser::symbol_kind::YYNTOKENS:
    case pascc::parse::Parser::symbol_kind::S_YYEMPTY:
    case pascc::parse::Parser::symbol_kind::S_YYEOF:
    case pascc::parse::Parser::symbol_kind::S_YYerror:
    case pascc::parse::Parser::symbol_kind::S_YYUNDEF: return "Unknown";
  }
}

void create_data(const std::vector<Parser::symbol_kind_type> &data, const std::string &filename)
{
  std::ofstream ofs(filename);
  for (const auto &d : data) {
    ofs << symbol_to_string(d) << '\n';
  }
}

TEST(LexerTest, keyword)
{
  // TODO(who): add more keyword

  // keyword data
  std::vector<Parser::symbol_kind_type> src_data = {
      Parser::symbol_kind::S_PROGRAM,
      Parser::symbol_kind::S_BEGIN,
      Parser::symbol_kind::S_END,
      Parser::symbol_kind::S_WRITELN,
  };

  // create data
  std::string filename = "keyword.txt";
  create_data(src_data, filename);

  // scan file
  pascc::parse::ParserDriver drv(filename, true, false);
  drv.location().initialize();
  drv.scan_begin();

  std::vector<Parser::symbol_kind_type> actual_result;
  while (true) {
    auto symbol = yylex(drv);
    if (symbol.kind_ == pascc::parse::Parser::symbol_kind::S_YYEOF) {
      break;
    }

    actual_result.push_back(symbol.kind_);
  }

  // compare
  EXPECT_EQ(src_data, actual_result);
  int size = static_cast<int>(src_data.size());
  for (int i = 0; i < size; ++i) {
    EXPECT_EQ(symbol_to_string(src_data[i]), symbol_to_string(actual_result[i]));
  }

  drv.scan_end();
}

TEST(LexerTest, id)
{
  // TODO(who): add identifier test
}

TEST(LexerTest, string)
{
  // TODO(who): add string test
  std::vector<Parser::symbol_kind_type> src_data = {
      Parser::symbol_kind::S_STR_LIT,

  };

  // create data
  std::string filename = "string_literal.txt";
  create_data(src_data, filename);

  // scan file
  pascc::parse::ParserDriver drv(filename, true, false);
  drv.location().initialize();
  drv.scan_begin();

  std::vector<Parser::symbol_kind_type> actual_result;
  while (true) {
    auto symbol = yylex(drv);
    if (symbol.kind_ == pascc::parse::Parser::symbol_kind::S_YYEOF) {
      break;
    }

    actual_result.push_back(symbol.kind_);
  }

  // compare
  EXPECT_EQ(src_data, actual_result);
  int size = static_cast<int>(src_data.size());
  for (int i = 0; i < size; ++i) {
    EXPECT_EQ(symbol_to_string(src_data[i]), symbol_to_string(actual_result[i]));
  }

  drv.scan_end();
}

TEST(LexerTest, number)
{
  // TODO(who): add number test
}

TEST(LexerTest, operator)
{
  // TODO(who): add operator test
}

TEST(LexerTest, delimiter)
{
  // TODO(who): add delimiter test
}

TEST(LexerTest, comment)
{
  // TODO(who): add comment test
}

// TODO(mfz&zh): more lexer test???