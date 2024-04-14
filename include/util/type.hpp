#pragma once

#include <memory>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "ast/visitor.hpp"

namespace pascc::util {

class SymType;

enum class BasicType
{
  INTEGER,
  REAL,
  BOOLEAN,
  CHAR,
  STRING
};

class BuiltInType
{
public:
  explicit BuiltInType(BasicType type)
    : type_(type)
  {}

  [[nodiscard]] auto type() const -> BasicType { return type_; }

private:
  BasicType type_;
};

class ArrayType
{
public:
  void add_period(int lower, int upper)
  {
    periods_.emplace_back(lower, upper);
  }

  [[nodiscard]] auto periods() const -> const std::vector<std::pair<int, int>> & { return periods_; }

  [[nodiscard]] auto base_type() const -> const SymType & { return *base_type_; }

private:
  std::vector<std::pair<int, int>> periods_;
  std::unique_ptr<SymType> base_type_;
};

class RecordType
{
public:
  [[nodiscard]] auto fields() const -> const std::unordered_map<std::string, std::unique_ptr<SymType>> & { return fields_; }

  void add_field(std::string name, std::unique_ptr<SymType> type)
  {
    fields_.emplace(std::move(name), std::move(type));
  }

private:
  std::unordered_map<std::string, std::unique_ptr<SymType>> fields_;
};

class SymType
{
public:
  enum class Type
  {
    BUILT_IN,
    ARRAY,
    RECORD
  };

  [[nodiscard]] auto type() const -> Type { return type_; }

  void set_type(BuiltInType type)
  {
    type_        = Type::BUILT_IN;
    actual_type_ = type;
  }

  void set_type(ArrayType type)
  {
    type_        = Type::ARRAY;
    actual_type_ = std::move(type);
  }

  void set_type(RecordType type)
  {
    type_        = Type::RECORD;
    actual_type_ = std::move(type);
  }

  [[nodiscard]] auto built_in_type() const -> const BuiltInType & { return std::get<BuiltInType>(actual_type_); }

  [[nodiscard]] auto array_type() const -> const ArrayType & { return std::get<ArrayType>(actual_type_); }

  [[nodiscard]] auto record_type() const -> const RecordType & { return std::get<RecordType>(actual_type_); }

private:
  Type type_;
  std::variant<BuiltInType, ArrayType, RecordType> actual_type_;
};


class TypeVisitor: public Visitor
{
public:
  SHARED_VISITOR_METHODS

  [[nodiscard]] auto type() -> SymType { return std::move(type_); }

private:
  SymType type_;
};

class TypeComparator
{
public:
  // TODO(): Implement this
  auto operator()(const SymType &lhs, const SymType &rhs) -> bool;

  // TODO(): Implement this
  static auto cast(const SymType &from, const SymType &to) -> bool;
};

}  // namespace pascc::util
