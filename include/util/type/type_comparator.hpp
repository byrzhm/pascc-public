#pragma once

#include "util/type/type.hpp"

namespace pascc::util {

class TypeComparator
{
public:
  /**
   * @brief 比较两个类型是否相等
   * 
   * @param lhs 类型1
   * @param rhs 类型2
   * @return true 类型相同
   * @return false 否则
   */
  auto operator()(const SymType &lhs, const SymType &rhs) -> bool;

  /**
   * @brief 比较两个类型是否可以转换，从from到to
   * 
   * @param from 类型1
   * @param to 类型2
   * @return true 可以转换，从from到to
   * @return false 否则
   */
  static auto cast(const SymType &from, const SymType &to) -> bool;
};

}  // namespace pascc::util