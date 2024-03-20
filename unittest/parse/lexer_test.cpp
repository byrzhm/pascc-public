#include <fstream>

#include "parse/parser_driver.hpp"
#include "parser.hpp"
#include "gtest/gtest.h"

using pascc::parse::Parser;

auto symbol_to_string(const Parser::symbol_type &symbol) -> std::string
{
  switch (symbol.kind_) {
    // token
    case pascc::parse::Parser::symbol_kind::S_AND: return "and";
    case pascc::parse::Parser::symbol_kind::S_ARRAY: return "array";
    case pascc::parse::Parser::symbol_kind::S_BEGIN: return "begin";
    case pascc::parse::Parser::symbol_kind::S_CASE: return "case";
    case pascc::parse::Parser::symbol_kind::S_CONST: return "const";
    case pascc::parse::Parser::symbol_kind::S_IDIV: return "div";
    case pascc::parse::Parser::symbol_kind::S_DO: return "do";
    case pascc::parse::Parser::symbol_kind::S_DOWNTO: return "downto";
    case pascc::parse::Parser::symbol_kind::S_ELSE: return "else";
    case pascc::parse::Parser::symbol_kind::S_END: return "end";
    case pascc::parse::Parser::symbol_kind::S_FILE: return "file";
    case pascc::parse::Parser::symbol_kind::S_FOR: return "for";
    case pascc::parse::Parser::symbol_kind::S_FUNCTION: return "function";
    case pascc::parse::Parser::symbol_kind::S_GOTO: return "goto";
    case pascc::parse::Parser::symbol_kind::S_IF: return "if";
    case pascc::parse::Parser::symbol_kind::S_IN: return "in";
    case pascc::parse::Parser::symbol_kind::S_LABEL: return "label";
    case pascc::parse::Parser::symbol_kind::S_MOD: return "mod";
    case pascc::parse::Parser::symbol_kind::S_NIL: return "nil";
    case pascc::parse::Parser::symbol_kind::S_NOT: return "not";
    case pascc::parse::Parser::symbol_kind::S_OF: return "of";
    case pascc::parse::Parser::symbol_kind::S_OR: return "or";
    case pascc::parse::Parser::symbol_kind::S_PACKED: return "packed";
    case pascc::parse::Parser::symbol_kind::S_PROCEDURE: return "procedure";
    case pascc::parse::Parser::symbol_kind::S_PROGRAM: return "program";
    case pascc::parse::Parser::symbol_kind::S_RECORD: return "record";
    case pascc::parse::Parser::symbol_kind::S_REPEAT: return "repeat";
    case pascc::parse::Parser::symbol_kind::S_SET: return "set";
    case pascc::parse::Parser::symbol_kind::S_THEN: return "then";
    case pascc::parse::Parser::symbol_kind::S_TO: return "to";
    case pascc::parse::Parser::symbol_kind::S_TYPE: return "type";
    case pascc::parse::Parser::symbol_kind::S_UNTIL: return "until";
    case pascc::parse::Parser::symbol_kind::S_VAR: return "var";
    case pascc::parse::Parser::symbol_kind::S_WHILE: return "while";
    case pascc::parse::Parser::symbol_kind::S_WITH: return "with";
    case pascc::parse::Parser::symbol_kind::S_READ: return "read";
    case pascc::parse::Parser::symbol_kind::S_READLN: return "readln";
    case pascc::parse::Parser::symbol_kind::S_WRITE: return "write";
    case pascc::parse::Parser::symbol_kind::S_WRITELN: return "writeln";
    case pascc::parse::Parser::symbol_kind::S_TRUE: return "true";
    case pascc::parse::Parser::symbol_kind::S_FALSE: return "false";
    case pascc::parse::Parser::symbol_kind::S_EXIT: return "exit";

    case pascc::parse::Parser::symbol_kind::S_FDIV: return "/";
    case pascc::parse::Parser::symbol_kind::S_SEMICOLON: return ";";
    case pascc::parse::Parser::symbol_kind::S_PERIOD: return ".";
    case pascc::parse::Parser::symbol_kind::S_LPAREN: return "(";
    case pascc::parse::Parser::symbol_kind::S_RPAREN: return ")";
    case pascc::parse::Parser::symbol_kind::S_LT: return "<";
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
    case pascc::parse::Parser::symbol_kind::S_AND: return "and";
    case pascc::parse::Parser::symbol_kind::S_ARRAY: return "array";
    case pascc::parse::Parser::symbol_kind::S_BEGIN: return "begin";
    case pascc::parse::Parser::symbol_kind::S_CASE: return "case";
    case pascc::parse::Parser::symbol_kind::S_CONST: return "const";
    case pascc::parse::Parser::symbol_kind::S_IDIV: return "div";
    case pascc::parse::Parser::symbol_kind::S_DO: return "do";
    case pascc::parse::Parser::symbol_kind::S_DOWNTO: return "downto";
    case pascc::parse::Parser::symbol_kind::S_ELSE: return "else";
    case pascc::parse::Parser::symbol_kind::S_END: return "end";
    case pascc::parse::Parser::symbol_kind::S_FILE: return "file";
    case pascc::parse::Parser::symbol_kind::S_FOR: return "for";
    case pascc::parse::Parser::symbol_kind::S_FUNCTION: return "function";
    case pascc::parse::Parser::symbol_kind::S_GOTO: return "goto";
    case pascc::parse::Parser::symbol_kind::S_IF: return "if";
    case pascc::parse::Parser::symbol_kind::S_IN: return "in";
    case pascc::parse::Parser::symbol_kind::S_LABEL: return "label";
    case pascc::parse::Parser::symbol_kind::S_MOD: return "mod";
    case pascc::parse::Parser::symbol_kind::S_NIL: return "nil";
    case pascc::parse::Parser::symbol_kind::S_NOT: return "not";
    case pascc::parse::Parser::symbol_kind::S_OF: return "of";
    case pascc::parse::Parser::symbol_kind::S_OR: return "or";
    case pascc::parse::Parser::symbol_kind::S_PACKED: return "packed";
    case pascc::parse::Parser::symbol_kind::S_PROCEDURE: return "procedure";
    case pascc::parse::Parser::symbol_kind::S_PROGRAM: return "program";
    case pascc::parse::Parser::symbol_kind::S_RECORD: return "record";
    case pascc::parse::Parser::symbol_kind::S_REPEAT: return "repeat";
    case pascc::parse::Parser::symbol_kind::S_SET: return "set";
    case pascc::parse::Parser::symbol_kind::S_THEN: return "then";
    case pascc::parse::Parser::symbol_kind::S_TO: return "to";
    case pascc::parse::Parser::symbol_kind::S_TYPE: return "type";
    case pascc::parse::Parser::symbol_kind::S_UNTIL: return "until";
    case pascc::parse::Parser::symbol_kind::S_VAR: return "var";
    case pascc::parse::Parser::symbol_kind::S_WHILE: return "while";
    case pascc::parse::Parser::symbol_kind::S_WITH: return "with";
    case pascc::parse::Parser::symbol_kind::S_READ: return "read";
    case pascc::parse::Parser::symbol_kind::S_READLN: return "readln";
    case pascc::parse::Parser::symbol_kind::S_WRITE: return "write";
    case pascc::parse::Parser::symbol_kind::S_WRITELN: return "writeln";
    case pascc::parse::Parser::symbol_kind::S_TRUE: return "true";
    case pascc::parse::Parser::symbol_kind::S_FALSE: return "false";
    case pascc::parse::Parser::symbol_kind::S_EXIT: return "exit";

    case pascc::parse::Parser::symbol_kind::S_FDIV: return "/";
    case pascc::parse::Parser::symbol_kind::S_SEMICOLON: return ";";
    case pascc::parse::Parser::symbol_kind::S_PERIOD: return ".";
    case pascc::parse::Parser::symbol_kind::S_LPAREN: return "(";
    case pascc::parse::Parser::symbol_kind::S_RPAREN: return ")";
    case pascc::parse::Parser::symbol_kind::S_LT: return "<";
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
      Parser::symbol_kind::S_AND,
      Parser::symbol_kind::S_ARRAY,
      Parser::symbol_kind::S_BEGIN,
      Parser::symbol_kind::S_CASE,
      Parser::symbol_kind::S_CONST,
      Parser::symbol_kind::S_IDIV,
      Parser::symbol_kind::S_DO,
      Parser::symbol_kind::S_DOWNTO,
      Parser::symbol_kind::S_ELSE,
      Parser::symbol_kind::S_END,
      Parser::symbol_kind::S_FILE,
      Parser::symbol_kind::S_FOR,
      Parser::symbol_kind::S_FUNCTION,
      Parser::symbol_kind::S_GOTO,
      Parser::symbol_kind::S_IF,
      Parser::symbol_kind::S_IN,
      Parser::symbol_kind::S_LABEL,
      Parser::symbol_kind::S_MOD,
      Parser::symbol_kind::S_NIL,
      Parser::symbol_kind::S_NOT,
      Parser::symbol_kind::S_OF,
      Parser::symbol_kind::S_OR,
      Parser::symbol_kind::S_PACKED,
      Parser::symbol_kind::S_PROCEDURE,
      Parser::symbol_kind::S_PROGRAM,
      Parser::symbol_kind::S_RECORD,
      Parser::symbol_kind::S_REPEAT,
      Parser::symbol_kind::S_SET,
      Parser::symbol_kind::S_THEN,
      Parser::symbol_kind::S_TO,
      Parser::symbol_kind::S_TYPE,
      Parser::symbol_kind::S_UNTIL,
      Parser::symbol_kind::S_VAR,
      Parser::symbol_kind::S_WHILE,
      Parser::symbol_kind::S_WITH,
      Parser::symbol_kind::S_READ,
      Parser::symbol_kind::S_READLN,
      Parser::symbol_kind::S_WRITE,
      Parser::symbol_kind::S_WRITELN,
      Parser::symbol_kind::S_TRUE,
      Parser::symbol_kind::S_FALSE,
      Parser::symbol_kind::S_EXIT
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
  EXPECT_EQ(src_data.size(), actual_result.size());
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