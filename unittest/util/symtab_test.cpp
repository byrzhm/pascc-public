#include "util/symtab.hpp"
#include "gtest/gtest.h"

using pascc::util::SymTab;

TEST(SymTabTest, probe)
{
  int arr[] = {0, 1, 2, 3, 4};
  SymTab<std::string, int *> symtab;
  symtab.insert("a", &arr[1]);
  symtab.insert("b", &arr[2]);
  symtab.enterScope();
  symtab.insert("a", &arr[3]);
  symtab.insert("c", &arr[4]);

  auto *a = symtab.probe("a");
  EXPECT_TRUE(a != nullptr);
  EXPECT_EQ(*a, 3);

  const auto *b = symtab.probe("b");
  EXPECT_EQ(b, nullptr);
}

TEST(SymTabTest, lookup)
{
  int arr[] = {0, 1, 2, 3, 4};
  SymTab<std::string, int *> symtab;
  symtab.insert("a", &arr[1]);
  symtab.insert("b", &arr[2]);
  symtab.enterScope();
  symtab.insert("a", &arr[3]);
  symtab.insert("c", &arr[4]);

  auto *a = symtab.lookup("a");  // 最近嵌套原则
  EXPECT_TRUE(a != nullptr);
  EXPECT_EQ(*a, 3);

  auto *b = symtab.lookup("b");
  EXPECT_TRUE(b != nullptr);
  EXPECT_EQ(*b, 2);
}
