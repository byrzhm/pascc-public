#include "util/symtab.hpp"
#include "gtest/gtest.h"

using pascc::util::SymTab;

TEST(SymTabTest, probe)
{
  SymTab<std::string, int> symtab;
  symtab.insert("a", 1);
  symtab.insert("b", 2);
  symtab.enterScope();
  symtab.insert("a", 3);
  symtab.insert("c", 4);

  const auto *a = symtab.probe("a");
  EXPECT_TRUE(a != nullptr);
  EXPECT_EQ(*a, 3);

  const auto *b = symtab.probe("b");
  EXPECT_EQ(b, nullptr);
}

TEST(SymTabTest, lookup)
{
  SymTab<std::string, int> symtab;
  symtab.insert("a", 1);
  symtab.insert("b", 2);
  symtab.enterScope();
  symtab.insert("a", 3);
  symtab.insert("c", 4);

  const auto *a = symtab.lookup("a");  // 最近嵌套原则
  EXPECT_TRUE(a != nullptr);
  EXPECT_EQ(*a, 3);

  const auto *b = symtab.lookup("b");
  EXPECT_TRUE(b != nullptr);
  EXPECT_EQ(*b, 2);
}
