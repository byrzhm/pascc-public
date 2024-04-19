#include "codegen/codegen.hpp"
#include "gtest/gtest.h"

#include "codegen_helper.hpp"

TEST(CodegenTest, example)
{
  testing::internal::CaptureStdout();
  pascc::codegen::CodegenVisitor visitor("");  // output to stdout
  auto program = create_example_ast();
  program->accept(visitor);

  std::string actual_code   = testing::internal::GetCapturedStdout();  // capture stdout
  actual_code               = strip(actual_code);

  std::string expected_code = read_expected_code("./data/codegen/1-basic.txt");
  expected_code             = strip(expected_code);

  EXPECT_EQ(actual_code, expected_code);
}

TEST(CodegenTest, helloworld)
{
  testing::internal::CaptureStdout();
  pascc::codegen::CodegenVisitor visitor("");  // output to stdout
  auto program = create_ast_helloworld();
  program->accept(visitor);

  std::string actual_code   = testing::internal::GetCapturedStdout();  // capture stdout
  actual_code               = strip(actual_code);

  std::string expected_code = read_expected_code("./data/codegen/2-helloworld.txt");
  expected_code             = strip(expected_code);

  EXPECT_EQ(actual_code, expected_code);
}