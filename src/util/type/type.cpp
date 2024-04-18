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

}  // namespace pascc::util