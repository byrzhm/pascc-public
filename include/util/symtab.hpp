#pragma once

namespace pascc::util {

class SymTab
{
public:
  /**
   * @brief 在当前作用域中查找符号
   */
  [[nodiscard]] auto probe(/* ... */) const -> bool;

  /**
   * @brief 在栈中存在的所有作用域中查找符号
   */
  [[nodiscard]] auto lookup(/* ... */) const -> bool;

  /**
   * @brief 在当前作用域中插入符号
   */
  void insert(/* ... */);

  /**
   * @brief 进入新的作用域
   */
  void enterScope();

  /**
   * @brief 退出当前作用域
   */
  void exitScope();

private:
  // ...
};

}  // namespace pascc::util