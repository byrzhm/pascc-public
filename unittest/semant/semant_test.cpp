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
    std::string filepath = "../../test-examples/wrong/redecl_error0.pas";
    SemantVisitor visitor;
    Driver::get().parse(filepath);
    Driver::get().program().accept(visitor);
    EXPECT_FALSE(visitor.isOk());
    for (const auto &msg : visitor.error_msgs()) {
      std::cout << msg << '\n';
    }
  }

  // TODO(any): add more test cases
  {
  }
}

// make sure update git submodule
// using `git submodule update --init --recursive` to update git submodule
TEST(SemantTest, typeError)
{
  {
    std::string filepath = "../../test-examples/wrong/type_error0.pas";
    SemantVisitor visitor;
    Driver::get().parse(filepath);
    Driver::get().program().accept(visitor);
    EXPECT_FALSE(visitor.isOk());
    for (const auto &msg : visitor.error_msgs()) {
      std::cout << msg << '\n';
    }
  }
}
