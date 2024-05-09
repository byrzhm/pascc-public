#pragma once

#include <unordered_map>
#include <vector>

namespace pascc::util {

/**
 * @brief 符号表
 */
template<typename SYM, typename DAT>
class SymTab
{
public:
  SymTab();

  /**
   * @brief 在当前作用域中查找符号
   *
   * @param name 符号名字
   * @return 符号表中对应的数据
   * @attention 在当前作用域
   */
  [[nodiscard]] auto probe(const SYM &name) const -> DAT;

  /**
   * @brief 在栈中存在的所有作用域中查找符号
   *
   * @param name 符号名字
   * @return 符号表中对应的数据
   * @attention 在所有作用域, 自内层作用域向外层作用域
   */
  [[nodiscard]] auto lookup(const SYM &name) const -> DAT;

  /**
   * @brief 在当前作用域中插入符号
   *
   * @param name 符号名字
   * @param data 符号表中对应的数据
   */
  void insert(const SYM &name, const DAT &data);

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