#include "util/type/type_comparator.hpp"


namespace pascc::util {
auto TypeComparator::operator()(
    const SymType &lhs,
    const SymType &rhs
) -> bool
{
  // TODO(): Implement this
  (void)lhs;
  (void)rhs;
  return false;
}

auto TypeComparator::cast(
    const SymType &from,
    const SymType &to
) -> bool
{
  // TODO(): Implement this
  (void)from;
  (void)to;
  return false;
}

}  // namespace pascc::util
