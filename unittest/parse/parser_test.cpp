#include "parse/parser_driver.hpp"
#include "gtest/gtest.h"

#include "parser_helper.hpp"

using pascc::parse::ParserDriver;

TEST(ParserTest, basic)
{
  std::string filepath = write_basic_test_file();
  ParserDriver drv(filepath, true, true);
  drv.parse();
  std::unique_ptr<Program> program = drv.program();
  ASSERT_NE(program, nullptr);

  std::cout << "\nParserTest::basic: program location:"
            << program->location() << '\n';
}