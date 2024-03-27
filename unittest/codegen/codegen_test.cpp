#include "codegen/codegen.hpp"
#include "gtest/gtest.h"

#include "codegen_helper.hpp"

TEST(CodegenTest, example)
{
  testing::internal::CaptureStdout();
  pascc::codegen::CodegenVisitor visitor("");
  auto program = create_example_ast();
  program->accept(visitor);

  std::string actual_code   = testing::internal::GetCapturedStdout();

  std::string expected_code = read_expected("./data/codegen/1-basic.txt");

  EXPECT_EQ(actual_code, expected_code);
}

TEST(CodegenTest, helloworld)
{
  testing::internal::CaptureStdout();
  pascc::codegen::CodegenVisitor visitor("");
  auto program = create_ast_helloworld();
  program->accept(visitor);
  std::string actual_code = testing::internal::GetCapturedStdout();

  // get expected code sample from text.
  std::string expected_code = read_expected("./data/codegen/2-helloworld.txt");

  EXPECT_EQ(actual_code, expected_code);
}