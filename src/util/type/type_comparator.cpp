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
    {
      if (rhs.eType() != SymType::Type::ARRAY) {
        return false;
      }
      if (lhs.arrayType().periods().size() != rhs.arrayType().periods().size()) {
        return false;
      }
      for (unsigned i = 0; i < lhs.arrayType().periods().size(); i++) {
        const auto &lperiod = lhs.arrayType().periods()[i];
        const auto &rperiod = rhs.arrayType().periods()[i];
        if (lperiod.second - lperiod.first != rperiod.second - rperiod.first) {
          return false;
        }
      }
      return (*this)(rhs.arrayType().baseType(), lhs.arrayType().baseType());
    }
    case SymType::Type::RECORD:
    {
      if (rhs.eType() != SymType::Type::RECORD) {
        return false;
      }
      if (lhs.recordType().fields().size() != rhs.recordType().fields().size()) {
        return false;
      }
      const auto &ll = lhs.recordType().fields();
      const auto &rr = rhs.recordType().fields();
      for (const auto &[field, type] : ll) {
        if (rr.at(field) != type) {
          return false;
        }
      }
      return true;
    }
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
  return from.eType() == util::SymType::Type::BUILT_IN && from.builtInType().type() == util::BasicType::INTEGER && to.eType() == util::SymType::Type::BUILT_IN && to.builtInType().type() == util::BasicType::REAL;
}

}  // namespace pascc::util
