#pragma once

#include <unordered_map>
#include <vector>

namespace pascc::util {

template<typename SYM, typename DAT>
class SymTab
{
public:
  SymTab();

  /**
   * @brief 在当前作用域中查找符号
   */
  [[nodiscard]] auto probe(SYM name) const -> const DAT *;

  /**
   * @brief 在栈中存在的所有作用域中查找符号
   */
  [[nodiscard]] auto lookup(SYM name) const -> const DAT *;

  /**
   * @brief 在当前作用域中插入符号
   */
  void insert(SYM name, DAT data);

  /**
   * @brief 进入新的作用域
   */
  void enterScope();

  /**
   * @brief 退出当前作用域
   */
  void exitScope();

private:
  std::vector<std::unordered_map<SYM, DAT>> symtab_;
};

}  // namespace pascc::util