#include "driver/driver.hpp"
#include "semant/semant.hpp"
#include "gtest/gtest.h"

using pascc::driver::Driver;
using pascc::semant::SemantVisitor;


TEST(SemantTest, redeclError0)
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

TEST(SemantTest, redeclError1)
{
  std::string filepath = "../../test-examples/wrong/redecl_error1.pas";
  SemantVisitor visitor;
  Driver::get().parse(filepath);
  Driver::get().program().accept(visitor);
  EXPECT_FALSE(visitor.isOk());
  for (const auto &msg : visitor.error_msgs()) {
    std::cout << msg << '\n';
  }
}

TEST(SemantTest, redeclError2)
{
  std::string filepath = "../../test-examples/wrong/redecl_error2.pas";
  SemantVisitor visitor;
  Driver::get().parse(filepath);
  Driver::get().program().accept(visitor);
  EXPECT_FALSE(visitor.isOk());
  for (const auto &msg : visitor.error_msgs()) {
    std::cout << msg << '\n';
  }
}

TEST(SemantTest, redeclError3)
{
  std::string filepath = "../../test-examples/wrong/redecl_error3.pas";
  SemantVisitor visitor;
  Driver::get().parse(filepath);
  Driver::get().program().accept(visitor);
  EXPECT_FALSE(visitor.isOk());
  for (const auto &msg : visitor.error_msgs()) {
    std::cout << msg << '\n';
  }
}

TEST(SemantTest, redeclError4)
{
  std::string filepath = "../../test-examples/wrong/redecl_error4.pas";
  SemantVisitor visitor;
  Driver::get().parse(filepath);
  Driver::get().program().accept(visitor);
  EXPECT_FALSE(visitor.isOk());
  for (const auto &msg : visitor.error_msgs()) {
    std::cout << msg << '\n';
  }
}

TEST(SemantTest, redeclError5)
{
  std::string filepath = "../../test-examples/wrong/redecl_error5.pas";
  SemantVisitor visitor;
  Driver::get().parse(filepath);
  Driver::get().program().accept(visitor);
  EXPECT_FALSE(visitor.isOk());
  for (const auto &msg : visitor.error_msgs()) {
    std::cout << msg << '\n';
  }
}

TEST(SemantTest, typeError0)
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

TEST(SemantTest, typeError1)
{
  std::string filepath = "../../test-examples/wrong/type_error1.pas";
  SemantVisitor visitor;
  Driver::get().parse(filepath);
  Driver::get().program().accept(visitor);
  EXPECT_FALSE(visitor.isOk());
  for (const auto &msg : visitor.error_msgs()) {
    std::cout << msg << '\n';
  }
}

TEST(SemantTest, typeError2)
{
  std::string filepath = "../../test-examples/wrong/type_error2.pas";
  SemantVisitor visitor;
  Driver::get().parse(filepath);
  Driver::get().program().accept(visitor);
  EXPECT_FALSE(visitor.isOk());
  for (const auto &msg : visitor.error_msgs()) {
    std::cout << msg << '\n';
  }
}

TEST(SemantTest, typeError3)
{
  std::string filepath = "../../test-examples/wrong/type_error3.pas";
  SemantVisitor visitor;
  Driver::get().parse(filepath);
  Driver::get().program().accept(visitor);
  EXPECT_FALSE(visitor.isOk());
  for (const auto &msg : visitor.error_msgs()) {
    std::cout << msg << '\n';
  }
}

TEST(SemantTest, typeError4)
{
  std::string filepath = "../../test-examples/wrong/type_error4.pas";
  SemantVisitor visitor;
  Driver::get().parse(filepath);
  Driver::get().program().accept(visitor);
  EXPECT_FALSE(visitor.isOk());
  for (const auto &msg : visitor.error_msgs()) {
    std::cout << msg << '\n';
  }
}

TEST(SemantTest, typeError5)
{
  std::string filepath = "../../test-examples/wrong/type_error5.pas";
  SemantVisitor visitor;
  Driver::get().parse(filepath);
  Driver::get().program().accept(visitor);
  EXPECT_FALSE(visitor.isOk());
  for (const auto &msg : visitor.error_msgs()) {
    std::cout << msg << '\n';
  }
}

TEST(SemantTest, typeError6)
{
  std::string filepath = "../../test-examples/wrong/type_error6.pas";
  SemantVisitor visitor;
  Driver::get().parse(filepath);
  Driver::get().program().accept(visitor);
  EXPECT_FALSE(visitor.isOk());
  for (const auto &msg : visitor.error_msgs()) {
    std::cout << msg << '\n';
  }
}

TEST(SemantTest, assignToConstError)
{
  std::string filepath = "../../test-examples/wrong/assign_to_const_error.pas";
  SemantVisitor visitor;
  Driver::get().parse(filepath);
  Driver::get().program().accept(visitor);
  EXPECT_FALSE(visitor.isOk());
  for (const auto &msg : visitor.error_msgs()) {
    std::cout << msg << '\n';
  }
}

TEST(SemantTest, opError)
{
  std::string filepath = "../../test-examples/wrong/op_error.pas";
  SemantVisitor visitor;
  Driver::get().parse(filepath);
  Driver::get().program().accept(visitor);
  EXPECT_FALSE(visitor.isOk());
  for (const auto &msg : visitor.error_msgs()) {
    std::cout << msg << '\n';
  }
}

TEST(SemantTest, passByRefError)
{
  std::string filepath = "../../test-examples/wrong/pass_by_ref_error.pas";
  SemantVisitor visitor;
  Driver::get().parse(filepath);
  Driver::get().program().accept(visitor);
  EXPECT_FALSE(visitor.isOk());
  for (const auto &msg : visitor.error_msgs()) {
    std::cout << msg << '\n';
  }
}

TEST(SemantTest, scopeError)
{
  std::string filepath = "../../test-examples/wrong/scope_error.pas";
  SemantVisitor visitor;
  Driver::get().parse(filepath);
  Driver::get().program().accept(visitor);
  EXPECT_FALSE(visitor.isOk());
  for (const auto &msg : visitor.error_msgs()) {
    std::cout << msg << '\n';
  }
}

TEST(SemantTest, subrangeError)
{
  std::string filepath = "../../test-examples/wrong/subrange_error.pas";
  SemantVisitor visitor;
  Driver::get().parse(filepath);
  Driver::get().program().accept(visitor);
  EXPECT_FALSE(visitor.isOk());
  for (const auto &msg : visitor.error_msgs()) {
    std::cout << msg << '\n';
  }
}

TEST(SemantTest, undefError)
{
  std::string filepath = "../../test-examples/wrong/undef_error.pas";
  SemantVisitor visitor;
  Driver::get().parse(filepath);
  Driver::get().program().accept(visitor);
  EXPECT_FALSE(visitor.isOk());
  for (const auto &msg : visitor.error_msgs()) {
    std::cout << msg << '\n';
  }
}