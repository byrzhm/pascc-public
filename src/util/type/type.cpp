#include "util/type/type.hpp"

namespace pascc::util {

auto SymType::IntegerType() -> SymType &
{
  static SymType integer_type{BuiltInType{BasicType::INTEGER}};
  return integer_type;
}

auto SymType::RealType() -> SymType &
{
  static SymType real_type{BuiltInType{BasicType::REAL}};
  return real_type;
}

auto SymType::BooleanType() -> SymType &
{
  static SymType boolean_type{BuiltInType{BasicType::BOOLEAN}};
  return boolean_type;
}

auto SymType::CharType() -> SymType &
{
  static SymType char_type{BuiltInType{BasicType::CHAR}};
  return char_type;
}

auto SymType::StringType() -> SymType &
{
  static SymType string_type{BuiltInType{BasicType::STRING}};
  return string_type;
}

auto SymType::clone() const -> std::unique_ptr<SymType>
{
  switch (type_) {
    case Type::NO_TYPE: return std::make_unique<SymType>();
    case Type::BUILT_IN: return std::make_unique<SymType>(builtInType());
    case Type::ARRAY: return std::make_unique<SymType>(arrayType());
    case Type::RECORD: return std::make_unique<SymType>(recordType());
    case Type::USER_DEFINED: return std::make_unique<SymType>(userDefinedType());
  }
  return {};
}

}  // namespace pascc::util