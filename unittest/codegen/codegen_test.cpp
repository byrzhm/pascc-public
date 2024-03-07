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

  std::string expected_code = get_example_expected_code();

  EXPECT_EQ(actual_code, expected_code);
}