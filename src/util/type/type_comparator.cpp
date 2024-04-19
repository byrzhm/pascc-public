#include "util/type/type_comparator.hpp"


namespace pascc::util {

auto TypeComparator::operator()(
    const SymType &lhs,
    const SymType &rhs
) -> bool
{
  if (lhs.eType() != rhs.eType()) {
    return false;
  }

  switch (lhs.eType()) {
    case SymType::Type::BUILT_IN:
      return lhs.builtInType().type() == rhs.builtInType().type();

    case SymType::Type::ARRAY:
      // TODO(): Implement this
    case SymType::Type::RECORD:
      // TODO(): Implement this
      return false;

    case SymType::Type::USER_DEFINED:
      return lhs.userDefinedType() == rhs.userDefinedType();

    case SymType::Type::NO_TYPE:
      return false;
  }
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
