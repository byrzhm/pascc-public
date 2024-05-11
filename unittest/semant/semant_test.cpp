#include "driver/driver.hpp"
#include "semant/semant.hpp"
#include "semant_helper.hpp"
#include "gtest/gtest.h"

using pascc::driver::Driver;
using pascc::semant::SemantVisitor;

TEST(SemantTest, example)
{
  SemantVisitor visitor;
  auto program = create_example_ast();
  program->accept(visitor);

  EXPECT_TRUE(visitor.isOk());
}


// make sure update git submodule
// using `git submodule update --init --recursive` to update git submodule
TEST(SemantTest, redeclError)
{
  {
    SemantVisitor visitor;
    Driver::Get().parse("../../test-examples/wrong/redecl_error0.pas");
    Driver::Get().program().accept(visitor);
    EXPECT_FALSE(visitor.isOk());
  }

  // TODO(any): add more test cases
  {
  }
}

// make sure update git submodule
// using `git submodule update --init --recursive` to update git submodule
TEST(SemantTest, DISABLED_typeError)
{
  {
    SemantVisitor visitor;
    Driver::Get().parse("../../test-examples/wrong/type_error0.pas");
    Driver::Get().program().accept(visitor);
    EXPECT_FALSE(visitor.isOk());
  }
}
