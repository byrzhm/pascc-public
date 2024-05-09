#pragma once

#include <memory>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

namespace pascc::util {

class SymType;

/**
 * @brief 基本数据类型枚举
 */
enum class BasicType
{
  NO_TYPE,  ///< 无类型
  INTEGER,  ///< 整型
  REAL,     ///< 浮点类型
  BOOLEAN,  ///< 布尔类型
  CHAR,     ///< 字符类型
  STRING    ///< 字符串类型
};

/**
 * @brief 表示内置类型
 */
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

/**
 * @brief 表示数组类型
 */
class ArrayType
{
public:
  void addPeriod(int lower, int upper)
  {
    periods_.emplace_back(lower, upper);
  }

  [[nodiscard]] auto periods() const -> const std::vector<std::pair<int, int>> & { return periods_; }

  [[nodiscard]] auto baseType() const -> const SymType & { return *base_type_; }

  /**
   * @brief 设置数组基本类型
   * 
   * @param base_type 
   */
  void setBaseType(SymType *base_type)
  {
    base_type_ = base_type;
  }

private:
  std::vector<std::pair<int, int>> periods_;
  SymType *base_type_;
};

/**
 * @brief 表示记录类型
 */
class RecordType
{
public:
  [[nodiscard]] auto fields() const -> const std::unordered_map<std::string, SymType *> & { return fields_; }

  /**
   * @brief 设置记录类型内各个类型和名字
   * 
   * @param name 
   * @param type 
   */
  void addField(std::string name, SymType *type)
  {
    fields_.emplace(std::move(name), type);
  }

private:
  std::unordered_map<std::string, SymType *> fields_;
};

/**
 * @brief 表示符号类型，包含各种类型
 * 
 */
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

  explicit SymType(std::string type)
    : type_(Type::USER_DEFINED)
    , actual_type_(std::move(type))
  {}

  /**
   * @brief 表示SymTab的不同类型
   */
  enum class Type
  {
    NO_TYPE,
    BUILT_IN,
    ARRAY,
    RECORD,
    USER_DEFINED
  };

  [[nodiscard]] auto eType() const -> Type { return type_; }

  [[nodiscard]] auto builtInType() const -> const BuiltInType & { return std::get<BuiltInType>(actual_type_); }

  [[nodiscard]] auto arrayType() const -> const ArrayType & { return std::get<ArrayType>(actual_type_); }

  [[nodiscard]] auto recordType() const -> const RecordType & { return std::get<RecordType>(actual_type_); }

  [[nodiscard]] auto userDefinedType() const -> const std::string & { return std::get<std::string>(actual_type_); }

  // Built-in types
  [[nodiscard]] static auto IntegerType() -> SymType &;
  [[nodiscard]] static auto RealType() -> SymType &;
  [[nodiscard]] static auto BooleanType() -> SymType &;
  [[nodiscard]] static auto CharType() -> SymType &;
  [[nodiscard]] static auto StringType() -> SymType &;
  [[nodiscard]] static auto NoType() -> SymType &;

  [[nodiscard]] auto clone() const -> std::unique_ptr<SymType>;

private:
  Type type_{Type::NO_TYPE};
  std::variant<BuiltInType, ArrayType, RecordType, std::string> actual_type_;
};

/**
 * @brief 表示变量类型
 */
class VarType
{
public:
  VarType(bool is_ref, SymType *type)
    : is_ref_(is_ref)
    , type_(type)
  {}

  [[nodiscard]] auto isRef() const -> bool { return is_ref_; }

  [[nodiscard]] auto symType() const -> const SymType &
  {
    if (type_ == nullptr) {
      throw std::runtime_error("VarType::symType: type is nullptr");
    }
    return *type_;
  }

private:
  bool is_ref_;
  SymType *type_;
};

/**
 * @brief 子程序(函数|过程)类型
 */
class SubprogType
{
public:
  using FormalVector = std::vector<std::pair<std::string, VarType *>>;
  SubprogType(bool is_func, SymType *return_type, FormalVector formal_params)
    : is_func_(is_func)
    , return_type_(return_type)
    , formal_params_(std::move(formal_params))
  {}

  [[nodiscard]] auto isFunc() const -> bool { return is_func_; }

  [[nodiscard]] auto returnType() const -> const SymType &
  {
    if (return_type_ == nullptr) {
      throw std::runtime_error("SubprogType::returnType: type is nullptr");
    }
    return *return_type_;
  }

  [[nodiscard]] auto formalParams() const -> const FormalVector & { return formal_params_; }

private:
  bool is_func_{false};
  SymType *return_type_{nullptr};
  FormalVector formal_params_;
};

}  // namespace pascc::util
