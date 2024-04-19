#pragma once

#include "util/type/type.hpp"

namespace pascc::util {

class TypeComparator
{
public:
  // TODO(): Implement this
  auto operator()(const SymType &lhs, const SymType &rhs) -> bool;

  // TODO(): Implement this
  static auto cast(const SymType &from, const SymType &to) -> bool;
};

}  // namespace pascc::util