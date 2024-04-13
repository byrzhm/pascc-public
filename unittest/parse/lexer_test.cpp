#include <fstream>
#include <random>

#include "parse/parser_driver.hpp"
#include "parser.hpp"
#include "gtest/gtest.h"

using pascc::parse::Parser;

/**
 * @brief 生成一个随机的pascal实数字符串
 * 
 * @return std::string 随机的pascal实数字符串
 */
auto gen_real_num_str() -> std::string
{
  std::string str;

  // 生成随机数，以确定实数字符串的形式
  std::mt19937 rng{std::random_device{}()};
  auto randomNumber = rng() % 5;

  switch (randomNumber) {
    case 0:
      str = std::to_string(rng() % 10) + "." + std::to_string(rng() % 10);
      break;
    case 1:
      str = "." + std::to_string(rng() % 10);
      break;
    case 2:
      str = std::to_string(rng() % 100) + ".";
      break;
    case 3:
      str = "." + std::to_string(rng() % 100) + "e-" + std::to_string(rng() % 10);
      break;
    case 4:
      str = std::to_string(rng() % 10) + "." + std::to_string(rng() % 10) + "e" + std::to_string(rng() % 10);
      break;
    default:
      break;
  }

  return str;
}

auto get_random_char() -> std::string
{
  const std::string CHARACTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_0123456789";

  std::string pChar;
  std::mt19937 rng{std::random_device{}()};

  pChar += "'";
  pChar += CHARACTERS[rng() % CHARACTERS.size()];
  pChar += "'";

  return pChar;
}

/**
 * @brief 生成一个随机的pascal字符串
 * 
 * @return std::string 随机的pascal字符串
 */
auto gen_random_str() -> std::string
{
  const std::string CHARACTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_";
  std::string pascalString;
  std::mt19937 rng{std::random_device{}()};

  pascalString += "'";

  for (unsigned i = 0; i < rng() % 20 + 2; ++i) {
    auto randomIndex  = rng() % CHARACTERS.size();
    pascalString     += CHARACTERS[randomIndex];
  }

  pascalString += "'";

  return pascalString;
}

auto get_random_id_str() -> std::string
{
  const std::string CHARACTERS       = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_0123456789";
  const std::string START_CHARACTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_";
  std::string pascalString;
  std::mt19937 rng{std::random_device{}()};

  pascalString += START_CHARACTERS[rng() % START_CHARACTERS.size()];

  for (unsigned i = 0; i < rng() % 20 + 2; ++i) {
    auto randomIndex  = rng() % CHARACTERS.size();
    pascalString     += CHARACTERS[randomIndex];
  }

  return pascalString;
}

auto symbol_to_string(const Parser::symbol_type &symbol) -> std::string
{
  switch (symbol.kind_) {
    // token
    case Parser::symbol_kind::S_AND: return "and";
    case Parser::symbol_kind::S_ARRAY: return "array";
    case Parser::symbol_kind::S_BEGIN: return "begin";
    case Parser::symbol_kind::S_CASE: return "case";
    case Parser::symbol_kind::S_CONST: return "const";
    case Parser::symbol_kind::S_IDIV: return "div";
    case Parser::symbol_kind::S_DO: return "do";
    case Parser::symbol_kind::S_DOWNTO: return "downto";
    case Parser::symbol_kind::S_ELSE: return "else";
    case Parser::symbol_kind::S_END: return "end";
    case Parser::symbol_kind::S_FILE: return "file";
    case Parser::symbol_kind::S_FOR: return "for";
    case Parser::symbol_kind::S_FUNCTION: return "function";
    case Parser::symbol_kind::S_GOTO: return "goto";
    case Parser::symbol_kind::S_IF: return "if";
    case Parser::symbol_kind::S_IN: return "in";
    case Parser::symbol_kind::S_LABEL: return "label";
    case Parser::symbol_kind::S_MOD: return "mod";
    case Parser::symbol_kind::S_NIL: return "nil";
    case Parser::symbol_kind::S_NOT: return "not";
    case Parser::symbol_kind::S_OF: return "of";
    case Parser::symbol_kind::S_OR: return "or";
    case Parser::symbol_kind::S_PACKED: return "packed";
    case Parser::symbol_kind::S_PROCEDURE: return "procedure";
    case Parser::symbol_kind::S_PROGRAM: return "program";
    case Parser::symbol_kind::S_RECORD: return "record";
    case Parser::symbol_kind::S_REPEAT: return "repeat";
    case Parser::symbol_kind::S_SET: return "set";
    case Parser::symbol_kind::S_THEN: return "then";
    case Parser::symbol_kind::S_TO: return "to";
    case Parser::symbol_kind::S_TYPE: return "type";
    case Parser::symbol_kind::S_UNTIL: return "until";
    case Parser::symbol_kind::S_VAR: return "var";
    case Parser::symbol_kind::S_WHILE: return "while";
    case Parser::symbol_kind::S_WITH: return "with";
    case Parser::symbol_kind::S_READ: return "read";
    case Parser::symbol_kind::S_READLN: return "readln";
    case Parser::symbol_kind::S_WRITE: return "write";
    case Parser::symbol_kind::S_WRITELN: return "writeln";
    case Parser::symbol_kind::S_TRUE: return "true";
    case Parser::symbol_kind::S_FALSE: return "false";
    case Parser::symbol_kind::S_EXIT: return "exit";

    case Parser::symbol_kind::S_PLUS: return "+";
    case Parser::symbol_kind::S_MINUS: return "-";
    case Parser::symbol_kind::S_MUL: return "*";
    case Parser::symbol_kind::S_FDIV: return "/";
    case Parser::symbol_kind::S_EQ: return "=";
    case Parser::symbol_kind::S_GT: return ">";
    case Parser::symbol_kind::S_LT: return "<";
    case Parser::symbol_kind::S_NE: return "<>";
    case Parser::symbol_kind::S_LE: return "<=";
    case Parser::symbol_kind::S_GE: return ">=";
    case Parser::symbol_kind::S_ASSIGN: return ":=";


    case Parser::symbol_kind::S_LSB: return "[";
    case Parser::symbol_kind::S_RSB: return "]";
    case Parser::symbol_kind::S_LPAREN: return "(";
    case Parser::symbol_kind::S_RPAREN: return ")";
    case Parser::symbol_kind::S_PERIOD: return ".";
    case Parser::symbol_kind::S_COMMA: return ",";
    case Parser::symbol_kind::S_COLON: return ":";
    case Parser::symbol_kind::S_SEMICOLON: return ";";
    case Parser::symbol_kind::S_RANGE: return "..";


    case Parser::symbol_kind::S_ID:
    case Parser::symbol_kind::S_STR_LIT: return symbol.value.as<std::string>();

    // non-terminal
    case Parser::symbol_kind::S_program:
    case Parser::symbol_kind::S_program_head:
    case Parser::symbol_kind::S_program_body:
    case Parser::symbol_kind::S_statement_block:
    case Parser::symbol_kind::S_statement:
    case Parser::symbol_kind::S_procedure_statement: return "nonterminal";

    case Parser::symbol_kind::YYNTOKENS:
    case Parser::symbol_kind::S_YYEMPTY:
    case Parser::symbol_kind::S_YYEOF:
    case Parser::symbol_kind::S_YYerror:
    case Parser::symbol_kind::S_YYUNDEF:
    default: return "Unknown";
  }
}

auto symbol_to_string(const Parser::symbol_kind_type &kind) -> std::string
{
  std::mt19937 rng{std::random_device{}()};
  switch (kind) {
    // token
    case Parser::symbol_kind::S_AND: return "and";
    case Parser::symbol_kind::S_ARRAY: return "array";
    case Parser::symbol_kind::S_BEGIN: return "begin";
    case Parser::symbol_kind::S_CASE: return "case";
    case Parser::symbol_kind::S_CONST: return "const";
    case Parser::symbol_kind::S_IDIV: return "div";
    case Parser::symbol_kind::S_DO: return "do";
    case Parser::symbol_kind::S_DOWNTO: return "downto";
    case Parser::symbol_kind::S_ELSE: return "else";
    case Parser::symbol_kind::S_END: return "end";
    case Parser::symbol_kind::S_FILE: return "file";
    case Parser::symbol_kind::S_FOR: return "for";
    case Parser::symbol_kind::S_FUNCTION: return "function";
    case Parser::symbol_kind::S_GOTO: return "goto";
    case Parser::symbol_kind::S_IF: return "if";
    case Parser::symbol_kind::S_IN: return "in";
    case Parser::symbol_kind::S_LABEL: return "label";
    case Parser::symbol_kind::S_MOD: return "mod";
    case Parser::symbol_kind::S_NIL: return "nil";
    case Parser::symbol_kind::S_NOT: return "not";
    case Parser::symbol_kind::S_OF: return "of";
    case Parser::symbol_kind::S_OR: return "or";
    case Parser::symbol_kind::S_PACKED: return "packed";
    case Parser::symbol_kind::S_PROCEDURE: return "procedure";
    case Parser::symbol_kind::S_PROGRAM: return "program";
    case Parser::symbol_kind::S_RECORD: return "record";
    case Parser::symbol_kind::S_REPEAT: return "repeat";
    case Parser::symbol_kind::S_SET: return "set";
    case Parser::symbol_kind::S_THEN: return "then";
    case Parser::symbol_kind::S_TO: return "to";
    case Parser::symbol_kind::S_TYPE: return "type";
    case Parser::symbol_kind::S_UNTIL: return "until";
    case Parser::symbol_kind::S_VAR: return "var";
    case Parser::symbol_kind::S_WHILE: return "while";
    case Parser::symbol_kind::S_WITH: return "with";
    case Parser::symbol_kind::S_READ: return "read";
    case Parser::symbol_kind::S_READLN: return "readln";
    case Parser::symbol_kind::S_WRITE: return "write";
    case Parser::symbol_kind::S_WRITELN: return "writeln";
    case Parser::symbol_kind::S_TRUE: return "true";
    case Parser::symbol_kind::S_FALSE: return "false";
    case Parser::symbol_kind::S_EXIT: return "exit";

    case Parser::symbol_kind::S_PLUS: return "+";
    case Parser::symbol_kind::S_MINUS: return "-";
    case Parser::symbol_kind::S_MUL: return "*";
    case Parser::symbol_kind::S_FDIV: return "/";
    case Parser::symbol_kind::S_EQ: return "=";
    case Parser::symbol_kind::S_GT: return ">";
    case Parser::symbol_kind::S_LT: return "<";
    case Parser::symbol_kind::S_NE: return "<>";
    case Parser::symbol_kind::S_LE: return "<=";
    case Parser::symbol_kind::S_GE: return ">=";
    case Parser::symbol_kind::S_ASSIGN: return ":=";


    case Parser::symbol_kind::S_LSB: return "[";
    case Parser::symbol_kind::S_RSB: return "]";
    case Parser::symbol_kind::S_LPAREN: return "(";
    case Parser::symbol_kind::S_RPAREN: return ")";
    case Parser::symbol_kind::S_PERIOD: return ".";
    case Parser::symbol_kind::S_COMMA: return ",";
    case Parser::symbol_kind::S_COLON: return ":";
    case Parser::symbol_kind::S_SEMICOLON: return ";";
    case Parser::symbol_kind::S_RANGE: return "..";

    case Parser::symbol_kind::S_ID: return get_random_id_str();
    case Parser::symbol_kind::S_STR_LIT: return gen_random_str();

    case Parser::symbol_kind::S_INT_NUM: return std::to_string(rng() % std::numeric_limits<int>::max()); ;
    case Parser::symbol_kind::S_REAL_NUM: return gen_real_num_str();

    case Parser::symbol_kind::S_CHAR:
      return get_random_char();
      // non-terminal
    case Parser::symbol_kind::S_program:
    case Parser::symbol_kind::S_program_head:
    case Parser::symbol_kind::S_program_body:
    case Parser::symbol_kind::S_statement_block:
    case Parser::symbol_kind::S_statement:
    case Parser::symbol_kind::S_procedure_statement: return "nonterminal";

    case Parser::symbol_kind::YYNTOKENS:
    case Parser::symbol_kind::S_YYEMPTY:
    case Parser::symbol_kind::S_YYEOF:
    case Parser::symbol_kind::S_YYerror:
    case Parser::symbol_kind::S_YYUNDEF:
    default: return "Unknown";
  }
}

void create_data(const std::vector<Parser::symbol_kind_type> &data, const std::string &filename)
{
  std::ofstream ofs(filename);
  for (const auto &d : data) {
    ofs << symbol_to_string(d) << '\n';
  }
}

template<typename T>
void create_data(const std::vector<T> &data, const std::string &filename)
{
  std::ofstream ofs(filename);
  for (const auto &d : data) {
    ofs << d << '\n';
  }
}



// To Disable: add prefix DISABLED_

TEST(LexerTest, DISABLED_keyword)
{
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
    std::cout << symbol.location << '\n';
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

TEST(LexerTest, DISABLED_id)
{
  std::vector<std::string> src_data = {
      "a123",
      "_",
      "A",
      "_dsf4qdsd"
  };

  // create data
  std::string filename = "id.txt";
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
    std::cout << symbol.location << '\n';
    actual_result.push_back(symbol.kind_);
    EXPECT_EQ(symbol.kind_, Parser::symbol_kind::S_ID);
  }
  EXPECT_EQ(src_data.size(), actual_result.size());
  drv.scan_end();
}

TEST(LexerTest, DISABLED_idRandom)
{
  int TSIZE = 10;
  std::vector<std::string> src_data(TSIZE);
  for (int i = 0; i < TSIZE; i++) {
    src_data[i] = get_random_id_str();
  }

  // create data
  std::string filename = "id.txt";
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

    std::cout << symbol.location << '\n';
    actual_result.push_back(symbol.kind_);
    EXPECT_EQ(symbol.kind_, Parser::symbol_kind::S_ID);
  }

  // compare
  EXPECT_EQ(src_data.size(), actual_result.size());

  drv.scan_end();
}

TEST(LexerTest, DISABLED_string)
{
  std::vector<std::string> src_data = {
      "'Its a quote within a string'",
      "'33'"
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
    std::cout << symbol.location << '\n';
    actual_result.push_back(symbol.kind_);
    EXPECT_EQ(symbol.kind_, Parser::symbol_kind::S_STR_LIT);
  }

  // compare
  EXPECT_EQ(src_data.size(), actual_result.size());

  drv.scan_end();
}

TEST(LexerTest, DISABLED_stringRandom)
{
  int TSIZE = 10;
  std::vector<std::string> src_data(TSIZE);
  for (int i = 0; i < TSIZE; i++) {
    src_data[i] = gen_random_str();
  }

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

    std::cout << symbol.location << '\n';
    actual_result.push_back(symbol.kind_);
    EXPECT_EQ(symbol.kind_, Parser::symbol_kind::S_STR_LIT);
  }

  // compare
  EXPECT_EQ(src_data.size(), actual_result.size());

  drv.scan_end();
}

TEST(LexerTest, DISABLED_integerNumber)
{
  std::mt19937 rng{std::random_device{}()};
  std::vector<int> src_data{1, 66, 888, 114514};

  std::string filename = "integer_number.txt";
  create_data(src_data, filename);

  // scan file
  pascc::parse::ParserDriver drv(filename, true, false);
  drv.location().initialize();
  drv.scan_begin();

  std::vector<int> actual_result;
  while (true) {
    auto symbol = yylex(drv);
    if (symbol.kind_ == pascc::parse::Parser::symbol_kind::S_YYEOF) {
      break;
    }
    std::cout << symbol.location << '\n';
    actual_result.push_back(symbol.value.as<int>());
  }

  // compare
  EXPECT_EQ(src_data.size(), actual_result.size());
  int size = static_cast<int>(src_data.size());
  for (int i = 0; i < size; ++i) {
    EXPECT_EQ(src_data[i], actual_result[i]);
  }

  drv.scan_end();
}

TEST(LexerTest, DISABLED_realNumber)
{
  std::vector<std::string> real_nums{"1.1",     "2.",  ".6",
                                     "114.514", "1e6", ".23e-9"};
  std::vector<double> src_data(real_nums.size());
  for (unsigned i = 0; i < real_nums.size(); ++i) {
    src_data[i] = std::stod(real_nums[i]);
  }

  std::string filename = "real_number.txt";
  create_data(real_nums, filename);

  // scan file
  pascc::parse::ParserDriver drv(filename, true, false);
  drv.location().initialize();
  drv.scan_begin();

  std::vector<double> actual_result;
  while (true) {
    auto symbol = yylex(drv);
    if (symbol.kind_ == pascc::parse::Parser::symbol_kind::S_YYEOF) {
      break;
    }
    std::cout << symbol.location << '\n';
    actual_result.push_back(symbol.value.as<double>());
  }

  // compare
  EXPECT_EQ(src_data.size(), actual_result.size());
  int size = static_cast<int>(src_data.size());
  for (int i = 0; i < size; ++i) {
    EXPECT_EQ(src_data[i], actual_result[i]);
  }

  drv.scan_end();
}

TEST(LexerTest, DISABLED_integerNumberRandom)
{
  std::mt19937 rng{std::random_device{}()};
  const int TSIZE = 10;
  std::vector<int> src_data(TSIZE);
  for (int i = 0; i < TSIZE; ++i) {
    src_data[i] = static_cast<int>(rng() % std::numeric_limits<int>::max());
  }

  std::string filename = "integer_number.txt";
  create_data(src_data, filename);

  // scan file
  pascc::parse::ParserDriver drv(filename, true, false);
  drv.location().initialize();
  drv.scan_begin();

  std::vector<int> actual_result;
  while (true) {
    auto symbol = yylex(drv);
    if (symbol.kind_ == pascc::parse::Parser::symbol_kind::S_YYEOF) {
      break;
    }
    std::cout << symbol.location << '\n';
    actual_result.push_back(symbol.value.as<int>());
  }

  // compare
  EXPECT_EQ(src_data.size(), actual_result.size());
  int size = static_cast<int>(src_data.size());
  for (int i = 0; i < size; ++i) {
    EXPECT_EQ(src_data[i], actual_result[i]);
  }

  drv.scan_end();
}

TEST(LexerTest, DISABLED_realNumberRandom)
{
  const int TSIZE = 10;
  std::vector<std::string> real_nums(TSIZE);
  std::vector<double> src_data(TSIZE);
  for (int i = 0; i < TSIZE; ++i) {
    real_nums[i] = gen_real_num_str();
    src_data[i]  = std::stod(real_nums[i]);
  }

  std::string filename = "real_number.txt";
  create_data(real_nums, filename);

  // scan file
  pascc::parse::ParserDriver drv(filename, true, false);
  drv.location().initialize();
  drv.scan_begin();

  std::vector<double> actual_result;
  while (true) {
    auto symbol = yylex(drv);
    if (symbol.kind_ == pascc::parse::Parser::symbol_kind::S_YYEOF) {
      break;
    }
    std::cout << symbol.location << '\n';
    actual_result.push_back(symbol.value.as<double>());
  }

  // compare
  EXPECT_EQ(src_data.size(), actual_result.size());
  int size = static_cast<int>(src_data.size());
  for (int i = 0; i < size; ++i) {
    EXPECT_EQ(src_data[i], actual_result[i]);
  }

  drv.scan_end();
}

TEST(LexerTest, DISABLED_operator)
{
  std::vector<Parser::symbol_kind_type> src_data = {
      Parser::symbol_kind::S_PLUS,
      Parser::symbol_kind::S_MINUS,
      Parser::symbol_kind::S_MUL,
      Parser::symbol_kind::S_FDIV,
      Parser::symbol_kind::S_EQ,
      Parser::symbol_kind::S_GT,
      Parser::symbol_kind::S_LT,
      Parser::symbol_kind::S_NE,
      Parser::symbol_kind::S_LE,
      Parser::symbol_kind::S_GE,
      Parser::symbol_kind::S_ASSIGN
  };

  // create data
  std::string filename = "operator.txt";
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
    std::cout << symbol.location << '\n';
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

TEST(LexerTest, DISABLED_delimiter)
{
  std::vector<Parser::symbol_kind_type> src_data = {
      Parser::symbol_kind::S_LSB,
      Parser::symbol_kind::S_RSB,
      Parser::symbol_kind::S_LPAREN,
      Parser::symbol_kind::S_RPAREN,
      Parser::symbol_kind::S_PERIOD,
      Parser::symbol_kind::S_COMMA,
      Parser::symbol_kind::S_COLON,
      Parser::symbol_kind::S_SEMICOLON,
      Parser::symbol_kind::S_RANGE
  };

  // create data
  std::string filename = "delimiter.txt";
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
    std::cout << symbol.location << '\n';
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

TEST(LexerTest, DISABLED_lineComment)
{
  std::vector<std::string> src_data = {
      "// sd5545'('djnwnqkn",
      "//",
      "//\t",
      "//Mamba出去"
  };

  std::string filename = "lineComment.txt";
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
    std::cout << symbol.location << '\n';
    actual_result.push_back(symbol.kind_);
  }

  // compare
  EXPECT_EQ(actual_result.size(), 0);

  drv.scan_end();
}

TEST(LexerTest, DISABLED_blockComment1)
{
  // { comment }

  std::vector<std::string> src_data = {
      "{ sd5545'('djnwnqkn}",
      "{\n\n\n}",
      "{\n男人\tHAHA\n\tWhat can I say,\nMAMBA OUT!}"
  };

  std::string filename = "blockComment1.txt";
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
    std::cout << symbol.location << '\n';
    actual_result.push_back(symbol.kind_);
  }

  // compare
  EXPECT_EQ(actual_result.size(), 0);

  drv.scan_end();
}

TEST(LexerTest, DISABLED_blockComment2)
{
  // (* comment *)

  std::vector<std::string> src_data = {
      "(* sd5545'('djnwnqkn*)",
      "(*\n\n\n*)",
      "(*\n男人\tHAHA\n\tWhat can I say,\nMAMBA OUT!*)",
      "(********)"
  };

  std::string filename = "blockComment2.txt";
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
    std::cout << symbol.location << '\n';
    actual_result.push_back(symbol.kind_);
  }

  // compare
  EXPECT_EQ(actual_result.size(), 0);

  drv.scan_end();
}

TEST(lexerTest, comprehensiveTest)
{
  std::vector<std::string> src_data = {
      "(*\n男人\tHAHA\n\tWhat can I say,\nMAMBA OUT!*)",
      "(********)",
      "114.5e4",
      "315",
      "downto",
      "or",
      "_or",
      "]",
      "<="
  };

  std::vector<Parser::symbol_kind_type> src_kind = {
      Parser::symbol_kind::S_REAL_NUM,
      Parser::symbol_kind::S_INT_NUM,
      Parser::symbol_kind::S_DOWNTO,
      Parser::symbol_kind::S_OR,
      Parser::symbol_kind::S_ID,
      Parser::symbol_kind::S_RSB,
      Parser::symbol_kind::S_LE
  };

  std::string filename = "comprehensiveTest.txt";
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
    std::cout << symbol.location << '\n';
    actual_result.push_back(symbol.kind_);
  }

  EXPECT_EQ(src_kind.size(), actual_result.size());
  int size = static_cast<int>(src_kind.size());
  for (int i = 0; i < size; i++) {
    EXPECT_EQ(src_kind[i], actual_result[i]);
  }
  drv.scan_end();
}

/*TEST(lexerTest, randomComprehensiveTest)
{
  int TSIZE = 100;
  std::vector<Parser::symbol_kind_type> src_data(TSIZE);
  std::mt19937 rng{std::random_device{}()};

  for (int i = 0; i < TSIZE; i++) {
    auto temp = rng() % 67 + 3;
    const Parser::symbol_type temp2
        src_data[i] = symbol_to_string(temp);
  }



  std::string filename = "comprehensiveTest.txt";
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
    std::cout << symbol.location << '\n';
    actual_result.push_back(symbol.kind_);
  }

  EXPECT_EQ(src_kind.size(), actual_result.size());
  int size = static_cast<int>(src_kind.size());
  for (int i = 0; i < size; i++) {
    EXPECT_EQ(src_kind[i], actual_result[i]);
  }
  drv.scan_end();
}*/

// TODO(mfz&zh): more lexer test???