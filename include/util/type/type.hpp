#pragma once

#include <memory>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

namespace pascc::util {

class SymType;

enum class BasicType
{
  NO_TYPE,
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
  void addPeriod(int lower, int upper)
  {
    periods_.emplace_back(lower, upper);
  }

  [[nodiscard]] auto periods() const -> const std::vector<std::pair<int, int>> & { return periods_; }

  [[nodiscard]] auto baseType() const -> const SymType & { return *base_type_; }

private:
  std::vector<std::pair<int, int>> periods_;
  std::unique_ptr<SymType> base_type_;
};

class RecordType
{
public:
  [[nodiscard]] auto fields() const -> const std::unordered_map<std::string, std::unique_ptr<SymType>> & { return fields_; }

  void addField(std::string name, std::unique_ptr<SymType> type)
  {
    fields_.emplace(std::move(name), std::move(type));
  }

private:
  std::unordered_map<std::string, std::unique_ptr<SymType>> fields_;
};

class SymType
{
public:
  SymType()
    : actual_type_(BuiltInType{BasicType::NO_TYPE})
  {}

  explicit SymType(BuiltInType type)
    : type_(Type::BUILT_IN)
    , actual_type_(type)
  {}

  explicit SymType(ArrayType type)
    : type_(Type::ARRAY)
    , actual_type_(std::move(type))
  {}

  explicit SymType(RecordType type)
    : type_(Type::RECORD)
    , actual_type_(std::move(type))
  {}

  enum class Type
  {
    NO_TYPE,
    BUILT_IN,
    ARRAY,
    RECORD
  };

  [[nodiscard]] auto type() const -> Type { return type_; }

  [[nodiscard]] auto builtInType() const -> const BuiltInType & { return std::get<BuiltInType>(actual_type_); }

  [[nodiscard]] auto arrayType() const -> const ArrayType & { return std::get<ArrayType>(actual_type_); }

  [[nodiscard]] auto recordType() const -> const RecordType & { return std::get<RecordType>(actual_type_); }

  // Built-in types
  [[nodiscard]] static auto IntegerType() -> SymType &;
  [[nodiscard]] static auto RealType() -> SymType &;
  [[nodiscard]] static auto BooleanType() -> SymType &;
  [[nodiscard]] static auto CharType() -> SymType &;
  [[nodiscard]] static auto StringType() -> SymType &;

private:
  Type type_{Type::NO_TYPE};
  std::variant<BuiltInType, ArrayType, RecordType> actual_type_;
};

class VarType
{
public:
  VarType(bool is_ref, SymType *type)
    : is_ref_(is_ref)
    , type_(type)
  {}

  [[nodiscard]] auto isRef() const -> bool { return is_ref_; }

  [[nodiscard]] auto type() const -> const SymType & { return *type_; }

private:
  bool is_ref_;
  SymType *type_;
};

class SubprogType
{
public:
  using FormalVector = std::vector<std::pair<std::string, VarType *>>;
  SubprogType(bool is_func, SymType *return_type, FormalVector formal_params)
    : is_func_(is_func)
    , return_type_(return_type)
    , formal_params_(std::move(formal_params))
  {}
  SubprogType(bool is_func, SymType *return_type)
    : is_func_(is_func)
    , return_type_(return_type)
  {
    formal_params_ = {{}};
  }
  [[nodiscard]] auto isFunc() const -> bool { return is_func_; }

  [[nodiscard]] auto returnType() const -> const SymType & { return *return_type_; }

  [[nodiscard]] auto formalParams() const -> const FormalVector & { return formal_params_; }

private:
  bool is_func_{false};
  SymType *return_type_{nullptr};
  FormalVector formal_params_;
};

}  // namespace pascc::util
