#include "semant/semant.hpp"
#include "semant_helper.hpp"
#include "gtest/gtest.h"

TEST(SemantTest, example)
{
  pascc::semant::SemantVisitor visitor;
  auto program = create_example_ast();
  program->accept(visitor);

  EXPECT_TRUE(visitor.isOk());
}