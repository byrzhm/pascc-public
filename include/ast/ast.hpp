#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "location.hh"
#include "util/type/type.hpp"

namespace pascc {

class Visitor;

namespace ast {

class Stmt;
class VarDecl;

class ConstDeclPart;
class TypeDeclPart;
class VarDeclPart;
class SubprogDeclPart;
class StmtPart;

/**
 * @brief 表示 Abstract Syntax Tree 的节点
 * @anchor ASTNode
 */
class ASTNode
{
public:
  /**
   * @brief 析构 ASTNode 对象
   */
  virtual ~ASTNode() = default;

  /**
   * @brief 访问者设计模式接口
   * @anchor accept
   * @see Visitor
   * @param v 传入的 visitor，
   *        真实类型一般是 Visitor 衍生类如 CodegenVisitor
   */
  virtual void accept(Visitor &v) = 0;

  /**
   * @brief 返回 AST 节点在文件中的对应位置
   * @anchor location
   * @return parse::location& AST 节点在文件中的对应位置
   */
  auto location() -> parse::location & { return loc_; }

private:
  parse::location loc_;  ///< 开始出现的行列数与结束时的行列数
};

/**
 * @brief 表示 Block 基类
 * @anchor Block 
 * @see ProgramBlock ProcBlock FuncBlock
 * @note block -> \n 
 *                constant_declaration_part type_declaration_part \n
 *                variable_declaration_part subprogram_declaration_part \n
 *                statement_part
 */
class Block: public ASTNode
{
public:
  /**
   * @brief 构建一个新的 Block 对象
   * 
   * @param const_decl_part 常量声明部分，不存在时为 nullptr
   * @param type_decl_part 类型声明部分，不存在时为 nullptr
   * @param var_decl_part 变量声明部分，不存在时为 nullptr
   * @param subprog_decl_part 子程序声明部分，不存在时为 nullptr
   * @param stmt_part 语句部分，不存在时为 nullptr
   *
   * @attention 如果 stmt_part 为空，那么源程序语法一定是错误的，会卡在语法分析阶段
   */
  Block(
      std::unique_ptr<ConstDeclPart> const_decl_part,
      std::unique_ptr<TypeDeclPart> type_decl_part,
      std::unique_ptr<VarDeclPart> var_decl_part,
      std::unique_ptr<SubprogDeclPart> subprog_decl_part,
      std::unique_ptr<StmtPart> stmt_part
  )
    : const_decl_part_(std::move(const_decl_part))
    , type_decl_part_(std::move(type_decl_part))
    , var_decl_part_(std::move(var_decl_part))
    , subprog_decl_part_(std::move(subprog_decl_part))
    , stmt_part_(std::move(stmt_part))
  {}

  /**
   * @brief 构造一个新的 Block 对象
   */
  Block(Block &&) = default;

  /**
   * @ref accept "见 ASTNode::accept"
   */
  void accept(Visitor &v) override;

  /**
   * 检查是否有 constant declaration part.
   *
   * @return true 有
   * @return false 没有
   */
  [[nodiscard]] auto hasConstDeclPart() -> bool { return const_decl_part_ != nullptr; }

  /**
   * 检查是否有 type declaration part.
   *
   * @return true 有
   * @return false 没有
   */
  [[nodiscard]] auto hasTypeDeclPart() -> bool { return type_decl_part_ != nullptr; }

  /**
   * 检查是否有 variable declaration part.
   *
   * @return true 有
   * @return false 没有
   */
  [[nodiscard]] auto hasVarDeclPart() -> bool { return var_decl_part_ != nullptr; }

  /**
   * 检查是否有 subprogram declaration part.
   *
   * @return true 有
   * @return false 没有
   */
  [[nodiscard]] auto hasSubprogDeclPart() -> bool { return subprog_decl_part_ != nullptr; }

  /**
   * 检查是否有 statement part.
   *
   * @return true 有
   * @return false 没有
   */
  [[nodiscard]] auto hasStmtPart() -> bool { return stmt_part_ != nullptr; }

  /**
   * 返回 `ConstDeclPart` 对象引用
   *
   * @return `ConstDeclPart` 对象引用.
   * @attention 使用之前必须使用 `hasConstDeclPart` 检查是否为空
   */
  [[nodiscard]] auto constDeclPart() -> ConstDeclPart & { return *const_decl_part_; }

  /**
   * 返回 `TypeDeclPart` 对象引用
   *
   * @return `TypeDeclPart` 对象引用.
   * @attention 使用之前必须使用 `hasTypeDeclPart` 检查是否为空
   */
  [[nodiscard]] auto typeDeclPart() -> TypeDeclPart & { return *type_decl_part_; }

  /**
   * 返回 `VarDeclPart` 对象引用
   *
   * @return `VarDeclPart` 对象引用.
   * @attention 使用之前必须使用 `hasVarDeclPart` 检查是否为空
   */
  [[nodiscard]] auto varDeclPart() -> VarDeclPart & { return *var_decl_part_; }

  /**
   * 返回 `SubprogDeclPart` 对象引用
   *
   * @return `SubprogDeclPart` 对象引用.
   * @attention 使用之前必须使用 `hasSubprogDeclPart` 检查是否为空
   */
  [[nodiscard]] auto subprogDeclPart() -> SubprogDeclPart & { return *subprog_decl_part_; }

  /**
   * 返回 `StmtPart` 对象引用
   *
   * @return `StmtPart` 对象引用.
   * @attention 使用之前必须使用 `hasStmtDeclPart` 检查是否为空
   */
  [[nodiscard]] auto stmtPart() -> StmtPart & { return *stmt_part_; }

private:
  std::unique_ptr<ConstDeclPart> const_decl_part_;      ///< constant declaration part
  std::unique_ptr<TypeDeclPart> type_decl_part_;        ///< type declaration part
  std::unique_ptr<VarDeclPart> var_decl_part_;          ///< variable declaration part
  std::unique_ptr<SubprogDeclPart> subprog_decl_part_;  ///< subprogram declaration part
  std::unique_ptr<StmtPart> stmt_part_;                 ///< statement part
};

/**
 * @brief 表示 expr 基类
 * @anchor Expr
 */
class Expr: public ASTNode
{
public:
  /**
   * 返回表达式的类型
   *
   * @return std::string 表达式的类型
   */
  [[nodiscard]] auto type() -> util::SymType & { return *type_; }

  /**
   * @brief 设置 type
   * 
   * @param type 设置的类型
   */
  void setType(std::unique_ptr<util::SymType> type) { type_ = std::move(type); }

  /**
   * @brief 是否是左值
   * 
   * @return true 是左值
   * @return false 不是左值
   */
  [[nodiscard]] auto isLvalue() const -> bool { return is_lvalue_; }

  /**
   * @brief 设置是否是左值
   * 
   * @param is_lvalue 是否是左值
   */
  void setIsLvalue(bool is_lvalue) { is_lvalue_ = is_lvalue; }

private:
  std::unique_ptr<util::SymType> type_;  ///< 表达式的类型
  bool is_lvalue_{false};                ///< 是否是左值
};

/**
 * @brief 表示布尔表达式
 * @anchor BoolExpr
 * @note bool_expr -> expr
 */
class BoolExpr: public Expr
{
public:
  /**
   * @brief 构建新的 BoolExpr 对象
   * 
   * @param expr 真正的 bool 表达式
   */
  explicit BoolExpr(std::unique_ptr<Expr> expr)
    : expr_(std::move(expr))
  {
    setIsLvalue(false);
  }

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  /**
   * 返回关联的表达式
   *
   * @return Expr& 表达式引用
   * @attention 在语义分析部分应该要进行检查，关联的表达式必须是boolean类型
   */
  [[nodiscard]] auto expr() -> Expr & { return *expr_; }

private:
  std::unique_ptr<Expr> expr_;  ///< 关联的布尔表达式
};

enum class BinOp
{
  PLUS,   ///< "+"
  MINUS,  ///< "-"
  MUL,    ///< "*"
  FDIV,   ///< "/"
  IDIV,   ///< "div"
  MOD,    ///< "mod"
  AND,    ///< "and"
  OR,     ///< "or"
  EQ,     ///< "="
  NE,     ///< "<>"
  LT,     ///< "<"
  GT,     ///< ">"
  LE,     ///< "<="
  GE      ///< ">="
};

/**
 * @brief 重载 `<<`，输出 `BinaryOp`
 * 
 * @param os 输出流
 * @param op 操作符
 * @return std::ostream& 输出流
 */
auto operator<<(std::ostream &os, BinOp op) -> std::ostream &;

/**
 * @brief 表示 binary expression.
 * @anchor BinaryExpr
 */
class BinaryExpr: public Expr
{
public:
  /**
   * Constructs a `BinaryExpr` object.
   * @param binop The binary operator.
   * @param lhs The left-hand side expression.
   * @param rhs The right-hand side expression.
   */
  BinaryExpr(BinOp binop, std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs)
    : binop_(binop)
    , lhs_(std::move(lhs))
    , rhs_(std::move(rhs))
  {
    setIsLvalue(false);
  }

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  /**
   * Gets the binary operator.
   * @return The binary operator.
   */
  [[nodiscard]] auto op() -> BinOp { return binop_; }

  /**
   * Gets the left-hand side expression.
   * @return The left-hand side expression.
   */
  [[nodiscard]] auto lhs() -> Expr & { return *lhs_; }

  /**
   * Gets the right-hand side expression.
   * @return The right-hand side expression.
   */
  [[nodiscard]] auto rhs() -> Expr & { return *rhs_; }

private:
  BinOp binop_;
  std::unique_ptr<Expr> lhs_;
  std::unique_ptr<Expr> rhs_;
};

enum class UnaryOp
{
  NOT,    ///< "not"
  MINUS,  ///< "-"
  PLUS    ///< "+"
};

/**
 * @brief 重载 `<<`，输出 `UnaryOp`
 * 
 * @param os 输出流
 * @param op 操作符
 * @return std::ostream& 输出流
 */
auto operator<<(std::ostream &os, UnaryOp op) -> std::ostream &;

/**
 * @brief 表示 unary expression.
 * @anchor UnaryExpr
 */
class UnaryExpr: public Expr
{
public:
  /**
   * Constructs a `UnaryExpr` object with the given expression.
   * @param expr The expression to be operated.
   */
  UnaryExpr(UnaryOp op, std::unique_ptr<Expr> expr)
    : op_(op)
    , expr_(std::move(expr))
  {
    setIsLvalue(false);
  }

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  /**
   * Returns the unary operator.
   * @return The unary operator.
   */
  [[nodiscard]] auto op() -> UnaryOp { return op_; }

  /**
   * Returns the expression to be operated.
   * @return The expression to be operated.
   */
  [[nodiscard]] auto expr() -> Expr & { return *expr_; }

private:
  UnaryOp op_;
  std::unique_ptr<Expr> expr_;
};

/**
 * @brief 表示 number 类
 * @anchor Number
 * @note num -> INT_NUM | REAL_NUM                    
 * @attention writeln(1) 中的 1 不是 Number 类型，而是 UnsignedConstant 类型
 */
class Number: public ASTNode
{
public:
  explicit Number(int value)
    : type_(&util::SymType::IntegerType())
    , value_(value)
  {}

  explicit Number(double value)
    : type_(&util::SymType::RealType())
    , value_(value)
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto type() -> util::SymType & { return *type_; }

  [[nodiscard]] auto value() -> std::variant<int, double> { return value_; }

private:
  util::SymType *type_;              ///< real 或 integer
  std::variant<int, double> value_;  ///< 使用 std::get<int>(value_) 或 std::get<double>(value_) 获取值
};

/**
 * @brief 表示字符串字面量
 * @anchor StringLiteral
 * @note string_literal -> STR_LIT
 */
class StringLiteral: public Expr
{
public:
  explicit StringLiteral(std::string string)
    : value_(std::move(string))
  {
    setType(std::make_unique<util::SymType>(util::BuiltInType{util::BasicType::STRING}));
    setIsLvalue(false);
  }

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto value() -> std::string & { return value_; }

private:
  std::string value_;
};

/**
 * @brief 表示无符号常量
 * @anchor UnsignedConstant
 * @note unsigned_constant -> \n
 *           num \n
 *           | CHAR \n
 *           | TRUE \n
 *           | FALSE
 */
class UnsignedConstant: public Expr
{
public:
  explicit UnsignedConstant(std::unique_ptr<Number> number)
  {
    if (number->type().actualType() != util::SymType::Type::BUILT_IN) {
      throw std::runtime_error("Number type should be built-in type");
    }

    auto built_in_type = number->type().builtInType();
    switch (built_in_type.type()) {
      case util::BasicType::INTEGER:
        setType(std::make_unique<util::SymType>(util::BuiltInType{util::BasicType::INTEGER}));
        value_ = std::get<int>(number->value());
        break;

      case util::BasicType::REAL:
        setType(std::make_unique<util::SymType>(util::BuiltInType{util::BasicType::REAL}));
        value_ = std::get<double>(number->value());
        break;

      default:
        throw std::runtime_error("Number is neither a integer nor a real");
        break;
    }
    setIsLvalue(false);
  }

  explicit UnsignedConstant(char value)
    : value_(value)
  {
    setType(std::make_unique<util::SymType>(util::BuiltInType{util::BasicType::CHAR}));
    setIsLvalue(false);
  }

  explicit UnsignedConstant(bool value)
    : value_(value)
  {
    setType(std::make_unique<util::SymType>(util::BuiltInType{util::BasicType::BOOLEAN}));
    setIsLvalue(false);
  }

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto value() -> std::variant<int, double, char, bool> { return value_; }

private:
  std::variant<int, double, char, bool> value_;
};

/**
 * @brief 表示函数调用
 * @anchor FuncCall
 * @note function_call -> \n 
 *       ID LPAREN RPAREN \n
 *       ID LPAREN expr_list RPAREN
 * @attention 没有参数的函数调用可以不带括号，但是语法分析阶段很难区分函数调用与变量名 \n 
 *            简单起见，函数调用必须带括号
 */
class FuncCall: public Expr
{
public:
  explicit FuncCall(std::string funcid)
    : funcid_(std::move(funcid))
  {
    setIsLvalue(false);
  }

  FuncCall(std::string funcid, std::vector<std::unique_ptr<Expr>> actuals)
    : funcid_(std::move(funcid))
    , actuals_(std::move(actuals))
  {
    setIsLvalue(false);
  }

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto funcid() -> std::string & { return funcid_; }

  [[nodiscard]] auto actuals() -> std::vector<std::unique_ptr<Expr>> & { return actuals_; }

private:
  std::string funcid_;
  std::vector<std::unique_ptr<Expr>> actuals_;
};

/**
 * @brief 表示可赋值的表达式
 * @anchor Assignable
 * @see AssignableId IndexedVar FieldDesignator
 * @note assignable -> ID \n
  *        | indexed_variable \n
  *        | field_designator
 */
class Assignable: public Expr
{
public:
  Assignable()
  {
    setIsLvalue(true);
  }
};

/**
 * @brief 表示一个可以赋值的标识符，如变量名与函数名
 * @anchor AssignableId
 * @see Assignable
 * @note assignable -> ID \n
 */
class AssignableId: public Assignable
{
public:
  explicit AssignableId(std::string id)
    : id_(std::move(id))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto id() -> std::string & { return id_; }

private:
  std::string id_;
};

/**
 * @brief 表示索引变量，如 a[10]
 * @anchor IndexedVar
 * @see Assignable
 * @note indexed_variable -> assignable LSB expr_list RSB
 */
class IndexedVar: public Assignable
{
public:
  IndexedVar(
      std::unique_ptr<Expr> assignable,
      std::vector<std::unique_ptr<Expr>> indices
  )
    : assignable_(std::move(assignable))
    , indices_(std::move(indices))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto assignable() -> Expr & { return *assignable_; }

  [[nodiscard]] auto indices() -> std::vector<std::unique_ptr<Expr>> & { return indices_; }

private:
  std::unique_ptr<Expr> assignable_;
  std::vector<std::unique_ptr<Expr>> indices_;
};

/**
 * @brief 结构体成员访问，point.x
 * @anchor FieldDesignator
 * @see Assignable
 * @note field_designator -> assignable PERIOD ID
 */
class FieldDesignator: public Assignable
{
public:
  FieldDesignator(
      std::unique_ptr<Expr> assignable,
      std::string field
  )
    : assignable_(std::move(assignable))
    , field_(std::move(field))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto assignable() -> Expr & { return *assignable_; }

  [[nodiscard]] auto field() -> std::string & { return field_; }

private:
  std::unique_ptr<Expr> assignable_;
  std::string field_;
};


/**
 * @brief 表示常量类
 * @anchor Constant
 * @note constant -> \n
 *        PLUS ID \n
 *        | MINUS ID \n
 *        | ID \n
 *        | num \n
 *        | PLUS num \n
 *        | MINUS num \n
 *        | CHAR \n
 *        | string_literal
 */
class Constant: public ASTNode
{
public:
  /**
   * @brief Construct a new Constant object
   * 
   * @param id 之前定义的常量的 id
   * @param sign 符号
   * @attention 如果引用的常量类型是 string 或 char，那么 sign 是无意义的,
   *            未来可以进行重构。如果常量的类型是引用，那么它的具体类型取决于
   *            引用常量的具体类型，如果引用常量还是个常量引用，那么继续递归。
   */
  explicit Constant(std::string id, int sign = 1)
    : sign_(sign)
    , type_("reference")
    , value_(std::move(id))
  {}

  /**
   * @brief Construct a new Constant object
   * 
   * @param number 传入的 number，可以是 integer 类型，也可以是 real 类型
   * @param sign 符号
   */
  explicit Constant(std::unique_ptr<Number> number, int sign = 1)
    : sign_(sign)
  {
    if (number->type().actualType() != util::SymType::Type::BUILT_IN) {
      throw std::runtime_error("Number type should be built-in type");
    }

    auto built_in_type = number->type().builtInType();
    switch (built_in_type.type()) {
      case util::BasicType::INTEGER:
        type_  = "integer";
        value_ = std::get<int>(number->value());
        break;

      case util::BasicType::REAL:
        type_  = "real";
        value_ = std::get<double>(number->value());
        break;

      default:
        throw std::runtime_error("Number is neither a integer nor a real");
        break;
    }
  }

  /**
   * @brief Construct a new Constant object
   * 
   * @param chr 字符
   * @param sign 符号
   */
  explicit Constant(char chr, int sign = 1)
    : sign_(sign)
    , type_("char")
    , value_(chr)
  {}

  /**
   * @brief Construct a new Constant object
   * 
   * @param string_literal 字符串字面值
   */
  explicit Constant(std::unique_ptr<StringLiteral> string_literal)
    : sign_(1)
    , type_("string")
    , value_(string_literal->value())
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  /**
   * @brief 返回 sign 的引用
   *
   * @return int& sign 的引用
   */
  [[nodiscard]] auto sign() -> int & { return sign_; }

  /**
   * @brief 返回 type 的引用
   *
   * @return std::string& type 的引用
   */
  [[nodiscard]] auto type() -> std::string & { return type_; }

  /**
   * @brief 返回 value 的引用
   * 
   * @return std::variant<std::string, int, double, char> value 的引用
   */
  [[nodiscard]] auto value() -> std::variant<std::string, int, double, char> { return value_; }

private:
  int sign_;          ///< 符号
  std::string type_;  ///< 常量类型

  /**
   * @brief value 使用 std::variant 是因为常量可以是多种类型
   * @code {.cpp}
   * if (type == "string" || "reference")
   *      val = std::get<std::string>(value_);
   * else if (type == "integer")
   *      val = std::get<int>(value_);
   * else if (type == "real")
   *      val = std::get<double>(value_);
   * else if (type == "char")
   *      val = std::get<char>(value_);
   * else
   *      throw std::runtime_error("Unexpected type");
   * @endcode
   */
  std::variant<std::string, int, double, char> value_;
};

/**
 * @brief 表示单独一个常量声明
 * @anchor ConstDecl
 * @note constant_declaration -> ID EQ constant
 */
class ConstDecl: public ASTNode
{
public:
  ConstDecl(std::string const_id, std::unique_ptr<Constant> constant)
    : const_id_(std::move(const_id))
    , constant_(std::move(constant))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto constId() -> std::string & { return const_id_; }

  [[nodiscard]] auto constant() -> Constant & { return *constant_; }

private:
  std::string const_id_;
  std::unique_ptr<Constant> constant_;
};

/**
 * @brief 表示常量声明部分
 * @anchor ConstDeclPart
 * @see Block
 * @note constant_declaration_part -> \n 
 *        ε | CONST constant_declarations SEMICOLON \n
 */
class ConstDeclPart: public ASTNode
{
public:
  /**
   * @brief 构造新的 `ConstDeclPart` 对象
   * 
   * @param const_decls `std::vector<std::unique_ptr<ConstDecl>>` 多个常量声明
   */
  explicit ConstDeclPart(std::vector<std::unique_ptr<ConstDecl>> const_decls)
    : const_decls_(std::move(const_decls))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto constDecls() -> std::vector<std::unique_ptr<ConstDecl>> & { return const_decls_; }

private:
  std::vector<std::unique_ptr<ConstDecl>> const_decls_;
};

//*******************************************************
//************************* Type ************************
//*******************************************************

/**
 * @brief 表示类型表示符
 * @anchor TypeDenoter
 * @see TypeId ArrayType RecordType
 * @note type_denoter -> \n
 *            type_identifier  \n
 *          | ARRAY LSB periods RSB OF type_denoter \n
 *          | RECORD field_list END
 */
class TypeDenoter: public ASTNode
{
public:
  /**
   * @brief 返回类型
   * 
   * @return util::SymType& 类型
   */
  [[nodiscard]] auto type() -> util::SymType & { return *type_; }

private:
  // TODO(): 语义分析时，将 type_ 设置为具体的类型
  std::unique_ptr<util::SymType> type_;  ///< 类型
};

/**
 * @brief 表示类型标识符，基本类型，以及用户声明声明的类型
 * @anchor TypeId
 * @see TypeDenoter
 * @note type_identifier -> ID
 */
class TypeId: public TypeDenoter
{
public:
  explicit TypeId(std::string id)
    : id_(std::move(id))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto id() -> std::string & { return id_; }

private:
  std::string id_;
};

/**
 * @brief 表示范围，出现在数组的声明中
 * @anchor Period
 * @note period -> constant RANGE constant
 */
class Period: public ASTNode
{
public:
  Period(std::unique_ptr<Constant> low, std::unique_ptr<Constant> high)
    : low_(std::move(low))
    , high_(std::move(high))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto low() -> Constant & { return *low_; }

  [[nodiscard]] auto high() -> Constant & { return *high_; }

private:
  std::unique_ptr<Constant> low_;
  std::unique_ptr<Constant> high_;
};

/**
 * @brief 表示数组类型
 * @anchor ArrayType
 * @see TypeDenoter
 * @note type_denoter -> ARRAY LSB periods RSB OF type_denoter
 */
class ArrayType: public TypeDenoter
{
public:
  ArrayType(
      std::unique_ptr<TypeDenoter> type,
      std::vector<std::unique_ptr<Period>> periods
  )
    : of_type_(std::move(type))
    , periods_(std::move(periods))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto ofType() -> TypeDenoter & { return *of_type_; }

  [[nodiscard]] auto periods() -> std::vector<std::unique_ptr<Period>> & { return periods_; }

private:
  std::unique_ptr<TypeDenoter> of_type_;
  std::vector<std::unique_ptr<Period>> periods_;
};

/**
 * @brief 表示记录类型，与C语言的结构体类似
 * @anchor RecordType
 * @see TypeDenoter
 * @note type_denoter -> RECORD field_list END
 */
class RecordType: public TypeDenoter
{
public:
  explicit RecordType(std::vector<std::unique_ptr<VarDecl>> fields)
    : fields_(std::move(fields))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto fields() -> std::vector<std::unique_ptr<VarDecl>> & { return fields_; }

private:
  std::vector<std::unique_ptr<VarDecl>> fields_;
};

/**
 * @brief 表示单独一个类型声明
 * @anchor TypeDecl
 * @note type_declaration -> ID EQ type_denoter
 */
class TypeDecl: public ASTNode
{
public:
  TypeDecl(std::string type_id, std::unique_ptr<TypeDenoter> type_denoter)
    : type_id_(std::move(type_id))
    , type_denoter_(std::move(type_denoter))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto typeId() -> std::string & { return type_id_; }

  [[nodiscard]] auto typeDenoter() -> TypeDenoter & { return *type_denoter_; }

private:
  std::string type_id_;
  std::unique_ptr<TypeDenoter> type_denoter_;
};

/**
 * @brief 表示类型声明部分
 * @anchor TypeDeclPart
 * @see Block
 * @note type_declaration_part -> ε | TYPE type_declarations SEMICOLON
 */
class TypeDeclPart: public ASTNode
{
public:
  explicit TypeDeclPart(std::vector<std::unique_ptr<TypeDecl>> type_decls)
    : type_decls_(std::move(type_decls))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto typeDecls() -> std::vector<std::unique_ptr<TypeDecl>> & { return type_decls_; }

private:
  std::vector<std::unique_ptr<TypeDecl>> type_decls_;
};

//*******************************************************
//************************* Var *************************
//*******************************************************

/**
 * @brief 表示单个变量声明
 * @anchor VarDecl
 * @note variable_declaration -> ID_LIST COLON type_denoter
 */
class VarDecl: public ASTNode
{
public:
  VarDecl(
      std::vector<std::string> id_list,
      std::unique_ptr<TypeDenoter> type
  )
    : id_list_(std::move(id_list))
    , type_(std::move(type))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto idList() -> std::vector<std::string> & { return id_list_; }

  [[nodiscard]] auto type() -> TypeDenoter & { return *type_; }

  [[nodiscard]] auto varType() -> util::VarType & { return *var_type_; };

  void setVarType(std::unique_ptr<util::VarType> var_type)
  {
    var_type_ = std::move(var_type);
  }

private:
  std::vector<std::string> id_list_;
  std::unique_ptr<TypeDenoter> type_;
  std::unique_ptr<util::VarType> var_type_;
};

/**
 * @brief 表示变量声明部分
 * @anchor VarDeclPart
 * @see Block
 * @note variable_declaration_part -> \n
  *        ε | VAR variable_declarations SEMICOLON
 */
class VarDeclPart: public ASTNode
{
public:
  explicit VarDeclPart(std::vector<std::unique_ptr<VarDecl>> var_decls)
    : var_decls_(std::move(var_decls))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto varDecls() -> std::vector<std::unique_ptr<VarDecl>> & { return var_decls_; }

private:
  std::vector<std::unique_ptr<VarDecl>> var_decls_;
};

//*******************************************************
//********************** Subprogram *********************
//*******************************************************

/**
 * @brief 表示单个子程序声明
 * @anchor SubprogDecl
 * @note subprogram_declaration -> procedure_declaration | function_declaration
 */
class SubprogDecl: public ASTNode
{
};

/**
 * @brief 表示形式参数
 * @anchor FormalParam
 * @see ValueParamSpec VarParamSpec
 * @note formal_parameter -> value_parameter_spec | var_parameter_spec
 */
class FormalParam: public ASTNode
{
public:
  /**
   * @brief 返回参数的类型
   * 
   * @return util::VarType& 参数的类型
   */
  [[nodiscard]] auto varType() -> util::VarType & { return *var_type_; }

  void setVarType(std::unique_ptr<util::VarType> var_type)
  {
    var_type_ = std::move(var_type);
  }

private:
  // TODO(夏博焕): 语义分析时，将 var_type_ 设置为具体的类型
  std::unique_ptr<util::VarType> var_type_;
};

/**
 * @brief 表示传值形式参数传递
 * @anchor ValueParamSpec
 * @see FormalParam
 * @note value_parameter_spec -> ID_LIST COLON type_denoter
 */
class ValueParamSpec: public FormalParam
{
public:
  ValueParamSpec(
      std::vector<std::string> id_list,
      std::unique_ptr<TypeDenoter> type
  )
    : id_list_(std::move(id_list))
    , type_(std::move(type))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto idList() -> std::vector<std::string> & { return id_list_; }

  [[nodiscard]] auto type() -> TypeDenoter & { return *type_; }

private:
  std::vector<std::string> id_list_;
  std::unique_ptr<TypeDenoter> type_;
};

/**
 * @brief 表示引用形式参数传递
 * @anchor VarParamSpec
 * @see FormalParam
 * @note var_parameter_spec -> VAR ID_LIST COLON type_denoter
 */
class VarParamSpec: public FormalParam
{
public:
  VarParamSpec(
      std::vector<std::string> id_list,
      std::unique_ptr<TypeDenoter> type
  )
    : id_list_(std::move(id_list))
    , type_(std::move(type))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto idList() -> std::vector<std::string> & { return id_list_; }

  [[nodiscard]] auto type() -> TypeDenoter & { return *type_; }

  [[nodiscard]] auto varType() -> util::VarType & { return *vartype_; }

private:
  std::vector<std::string> id_list_;
  std::unique_ptr<TypeDenoter> type_;
  std::unique_ptr<util::VarType> vartype_;
};

/**
 * @brief 表示过程声明的头部信息
 * @anchor ProcHead
 * @note procedure_head -> PROCEDURE ID SEMICOLON \n 
 *        | PROCEDURE ID LPAREN formal_parameter_list RPAREN SEMICOLON
 */
class ProcHead: public ASTNode
{
public:
  explicit ProcHead(std::string proc_id)
    : proc_id_(std::move(proc_id))
  {}

  ProcHead(
      std::string proc_id,
      std::vector<std::unique_ptr<FormalParam>> formal_params
  )
    : proc_id_(std::move(proc_id))
    , formal_params_(std::move(formal_params))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto procId() -> std::string & { return proc_id_; }

  [[nodiscard]] auto formalParams() -> std::vector<std::unique_ptr<FormalParam>> & { return formal_params_; }

  [[nodiscard]] auto procType() -> util::SubprogType & { return *proc_type_; }

  void setProcType(std::unique_ptr<util::SubprogType> proc_type) { proc_type_ = std::move(proc_type); }

private:
  std::string proc_id_;
  std::vector<std::unique_ptr<FormalParam>> formal_params_;
  std::unique_ptr<util::SubprogType> proc_type_;
};

/**
 * @brief 表示过程块
 * @anchor ProcBlock
 * @see Block
 * @note procedure_block -> block
 */
class ProcBlock: public Block
{
public:
  explicit ProcBlock(Block block)
    : Block(std::move(block))
  {}

  explicit ProcBlock(std::unique_ptr<Block> block)
    : Block(std::move(*block))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;
};

/**
 * @brief 表示单个过程声明
 * @anchor ProcDecl
 * @see SubprogDecl
 * @note procedure_declaration -> procedure_head procedure_block
 */
class ProcDecl: public SubprogDecl
{
public:
  ProcDecl(
      std::unique_ptr<ProcHead> head,
      std::unique_ptr<ProcBlock> block
  )
    : head_(std::move(head))
    , block_(std::move(block))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto head() -> ProcHead & { return *head_; }

  [[nodiscard]] auto block() -> ProcBlock & { return *block_; }

private:
  std::unique_ptr<ProcHead> head_;
  std::unique_ptr<ProcBlock> block_;
};

/**
 * @brief 表示函数头部信息
 * @anchor FuncHead
 * @note function_head -> FUNCTION ID COLON type_denoter SEMICOLON \n
  *        | FUNCTION ID LPAREN formal_parameter_list RPAREN COLON type_denoter SEMICOLON
 */
class FuncHead: public ASTNode
{
public:
  FuncHead(
      std::string func_id,
      std::unique_ptr<TypeDenoter> return_type
  )
    : func_id_(std::move(func_id))
    , return_type_(std::move(return_type))
  {}

  FuncHead(
      std::string func_id,
      std::vector<std::unique_ptr<FormalParam>> formal_params,
      std::unique_ptr<TypeDenoter> return_type
  )
    : func_id_(std::move(func_id))
    , formal_params_(std::move(formal_params))
    , return_type_(std::move(return_type))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto funcId() -> std::string & { return func_id_; }

  [[nodiscard]] auto formalParams() -> std::vector<std::unique_ptr<FormalParam>> & { return formal_params_; }

  [[nodiscard]] auto returnType() -> TypeDenoter & { return *return_type_; }

  [[nodiscard]] auto funcType() -> util::SubprogType & { return *func_type_; }

  void setFuncType(std::unique_ptr<util::SubprogType> func_type) { func_type_ = std::move(func_type); }

  [[nodiscard]] auto funcIdType() -> util::VarType & { return *func_id_type_; }

  void setFuncIdType(std::unique_ptr<util::VarType> func_id_type) { func_id_type_ = std::move(func_id_type); }

private:
  std::string func_id_;
  std::vector<std::unique_ptr<FormalParam>> formal_params_;
  std::unique_ptr<TypeDenoter> return_type_;

  std::unique_ptr<util::VarType> func_id_type_;
  std::unique_ptr<util::SubprogType> func_type_;
};

/**
 * @brief 表示函数块
 * @anchor FuncBlock
 * @see Block
 * @note function_block -> block
 */
class FuncBlock: public Block
{
public:
  explicit FuncBlock(Block block)
    : Block(std::move(block))
  {}

  explicit FuncBlock(std::unique_ptr<Block> block)
    : Block(std::move(*block))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;
};

/**
 * @brief 表示单个函数声明
 * @anchor FuncDecl
 * @see SubprogDecl
 * @note function_declaration -> function_head function_block
 */
class FuncDecl: public SubprogDecl
{
public:
  FuncDecl(
      std::unique_ptr<FuncHead> head,
      std::unique_ptr<FuncBlock> block
  )
    : head_(std::move(head))
    , block_(std::move(block))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto head() -> FuncHead & { return *head_; }

  [[nodiscard]] auto block() -> FuncBlock & { return *block_; }

private:
  std::unique_ptr<FuncHead> head_;
  std::unique_ptr<FuncBlock> block_;
};

/**
 * @brief 表示子程序声明部分
 * @anchor SubprogDeclPart
 * @see Block
 * @note subprogram_declaration_part -> \n
  *        ε | subprogram_declarations SEMICOLON
 */
class SubprogDeclPart: public ASTNode
{
public:
  explicit SubprogDeclPart(
      std::vector<std::unique_ptr<SubprogDecl>> subprog_decls
  )
    : subprog_decls_(std::move(subprog_decls))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto subprogDecls() -> std::vector<std::unique_ptr<SubprogDecl>> & { return subprog_decls_; }

private:
  std::vector<std::unique_ptr<SubprogDecl>> subprog_decls_;
};

//*******************************************************
//********************** Statement **********************
//*******************************************************

/**
 * @brief 表示一个语句
 * @anchor Stmt
 * @note statement -> simple_statement | structured_statement
 */
class Stmt: public ASTNode
{
};

/**
 * @brief 表示一个简单语句
 * @anchor SimpleStmt
 * @see AssignStmt ProcCallStmt
 * @note simple_statement -> empty_statement | assignment_statement | procedure_call_statement
 */
class SimpleStmt: public Stmt
{
};

/**
 * @brief 表示一个赋值语句
 * @anchor AssignStmt
 * @see SimpleStmt
 * @note assignment_statement -> assignable ASSIGN expression
 */
class AssignStmt: public SimpleStmt
{
public:
  AssignStmt(
      std::unique_ptr<Expr> lhs,
      std::unique_ptr<Expr> rhs
  )
    : lhs_(std::move(lhs))
    , rhs_(std::move(rhs))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto lhs() -> Expr & { return *lhs_; }

  [[nodiscard]] auto rhs() -> Expr & { return *rhs_; }

private:
  std::unique_ptr<Expr> lhs_;
  std::unique_ptr<Expr> rhs_;
};

/**
 * @brief 表示一个过程调用语句
 * @anchor ProcCallStmt
 * @see SimpleStmt
 * @note procedure_call_statement -> \n
 *              ID \n
 *              | ID LPAREN RPAREN \n
 *              | ID LPAREN expr_list RPAREN \n
 *              | write_statement \n
 *              | writeln_statement \n
 *              | read_statement \n
 *              | readln_statement \n
 *              | exit_statement
 */
class ProcCallStmt: public SimpleStmt
{
public:
  explicit ProcCallStmt(std::string proc_id)
    : proc_id_(std::move(proc_id))
  {}

  ProcCallStmt(
      std::string proc_id,
      std::vector<std::unique_ptr<Expr>> actuals
  )
    : proc_id_(std::move(proc_id))
    , actuals_(std::move(actuals))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto procId() -> std::string & { return proc_id_; }

  [[nodiscard]] auto actuals() -> std::vector<std::unique_ptr<Expr>> & { return actuals_; }

private:
  std::string proc_id_;
  std::vector<std::unique_ptr<Expr>> actuals_;
};

/**
 * @brief 表示特殊过程调用 Read
 * @anchor ReadStmt
 * @see ProcCallStmt
 * @note read_statement -> READ LPAREN assignable_list RPAREN
 */
class ReadStmt: public ProcCallStmt
{
public:
  ReadStmt()
    : ProcCallStmt("read")
  {}

  explicit ReadStmt(std::vector<std::unique_ptr<Expr>> assignables)
    : ProcCallStmt("read", std::move(assignables))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;
};

/**
 * @brief 表示特殊过程调用 Write
 * @anchor WriteStmt
 * @see ProcCallStmt
 * @note write_statement -> WRITE LPAREN expr_list RPAREN
 */
class WriteStmt: public ProcCallStmt
{
public:
  WriteStmt()
    : ProcCallStmt("write")
  {}

  explicit WriteStmt(std::vector<std::unique_ptr<Expr>> exprs)
    : ProcCallStmt("write", std::move(exprs))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;
};

/**
 * @brief 表示特殊过程调用 Readln
 * @anchor ReadlnStmt
 * @see ProcCallStmt
 * @note readln_statement -> READLN LPAREN assignable_list RPAREN
 */
class ReadlnStmt: public ProcCallStmt
{
public:
  ReadlnStmt()
    : ProcCallStmt("readln")
  {}

  explicit ReadlnStmt(std::vector<std::unique_ptr<Expr>> assignables)
    : ProcCallStmt("readln", std::move(assignables))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;
};

/**
 * @brief 表示特殊过程调用 Writeln
 * @anchor WritelnStmt
 * @see ProcCallStmt
 * @note writeln_statement -> \n 
 *          WRITELN \n
 *          | WRITELN LPAREN RPAREN \n
 *          | WRITELN LPAREN expr_list RPAREN
 */
class WritelnStmt: public ProcCallStmt
{
public:
  WritelnStmt()
    : ProcCallStmt("writeln")
  {}

  explicit WritelnStmt(std::vector<std::unique_ptr<Expr>> exprs)
    : ProcCallStmt("writeln", std::move(exprs))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;
};

/**
 * @brief 表示特殊过程调用 Exit
 * @anchor ExitStmt
 * @see ProcCallStmt
 * @note exit_statement -> \n 
 *            EXIT \n 
 *            | EXIT LPAREN RPAREN \n
 *            | EXIT LPAREN expr_list RPAREN
 */
class ExitStmt: public ProcCallStmt
{
public:
  ExitStmt()
    : ProcCallStmt("exit")
  {}

  explicit ExitStmt(std::vector<std::unique_ptr<Expr>> exprs)
    : ProcCallStmt("exit", std::move(exprs))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;
};

/**
 * @brief 表示结构化语句
 * @anchor StructuredStmt
 * @see ConditionalStmt RepetitiveStmt
 * @note structured_statement -> \n 
 *         compound_statement \n
 *         | conditional_statement \n
 *         | repetitive_statement
 */
class StructuredStmt: public Stmt
{
};

/**
 * @brief 表示条件语句
 * @anchor ConditionalStmt
 * @see StructuredStmt
 * @see IfStmt CaseStmt
 * @note conditional_statement -> if_statement | case_statement
 */
class ConditionalStmt: public StructuredStmt
{
};

/**
 * @brief 表示If语句
 * @anchor IfStmt
 * @see ConditionalStmt
 * @note if_statement -> IF bool_expr THEN statement else_part \n
 *       else_part -> ELSE statement | ε
 */
class IfStmt: public ConditionalStmt
{
public:
  IfStmt(
      std::unique_ptr<BoolExpr> cond,
      std::unique_ptr<Stmt> then,
      std::unique_ptr<Stmt> _else
  )
    : cond_(std::move(cond))
    , then_(std::move(then))
    , else_(std::move(_else))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto cond() -> Expr & { return *cond_; }

  [[nodiscard]] auto then() -> Stmt & { return *then_; }

  [[nodiscard]] auto hasElse() -> bool { return else_ != nullptr; }

  [[nodiscard]] auto Else() -> Stmt & { return *else_; }

private:
  std::unique_ptr<BoolExpr> cond_;
  std::unique_ptr<Stmt> then_;
  std::unique_ptr<Stmt> else_;
};

/**
 * @brief 表示一个Case分支
 * @anchor CaseListElement
 * @note case_list_element -> constant_list COLON statement
 */
class CaseListElement: public ASTNode
{
public:
  CaseListElement(
      std::vector<std::unique_ptr<Constant>> constants,
      std::unique_ptr<Stmt> stmt
  )
    : constants_(std::move(constants))
    , stmt_(std::move(stmt))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto constants() -> std::vector<std::unique_ptr<Constant>> & { return constants_; }

  [[nodiscard]] auto stmt() -> Stmt & { return *stmt_; }

private:
  std::vector<std::unique_ptr<Constant>> constants_;
  std::unique_ptr<Stmt> stmt_;
};

/**
 * @brief 表示Case语句
 * @anchor CaseStmt
 * @see ConditionalStmt
 * @note case_statement -> CASE expr OF case_list_elements opt_semicolon END
 */
class CaseStmt: public ConditionalStmt
{
public:
  CaseStmt(
      std::unique_ptr<Expr> expr,
      std::vector<std::unique_ptr<CaseListElement>> case_list
  )
    : expr_(std::move(expr))
    , case_list_(std::move(case_list))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto expr() -> Expr & { return *expr_; }

  [[nodiscard]] auto caseList() -> std::vector<std::unique_ptr<CaseListElement>> & { return case_list_; }

private:
  std::unique_ptr<Expr> expr_;
  std::vector<std::unique_ptr<CaseListElement>> case_list_;
};

/**
 * @brief 表示重复语句
 * @anchor RepetitveStmt
 * @see StructuredStmt
 * @see RepeatStmt WhileStmt ForStmt CompoundStmt
 * @note repetitive_statement -> repeat_statement | while_statement | for_statement
 */
class RepetitiveStmt: public StructuredStmt
{
};

/**
 * @brief 表示Repeat语句
 * @anchor RepeatStmt
 * @see RepetitiveStmt
 * @note repeat_statement -> REPEAT statement_list UNTIL bool_expr
 */
class RepeatStmt: public RepetitiveStmt
{
public:
  RepeatStmt(
      std::vector<std::unique_ptr<Stmt>> body,
      std::unique_ptr<BoolExpr> cond
  )
    : body_(std::move(body))
    , cond_(std::move(cond))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto body() -> std::vector<std::unique_ptr<Stmt>> & { return body_; }

  [[nodiscard]] auto cond() -> Expr & { return *cond_; }

private:
  std::vector<std::unique_ptr<Stmt>> body_;
  std::unique_ptr<BoolExpr> cond_;
};

/**
 * @brief 表示While语句
 * @anchor WhileStmt
 * @see RepetitiveStmt
 * @note while_statement -> WHILE bool_expr DO statement
 */
class WhileStmt: public RepetitiveStmt
{
public:
  WhileStmt(
      std::unique_ptr<BoolExpr> cond,
      std::unique_ptr<Stmt> body
  )
    : cond_(std::move(cond))
    , body_(std::move(body))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto cond() -> Expr & { return *cond_; }

  [[nodiscard]] auto body() -> Stmt & { return *body_; }

private:
  std::unique_ptr<BoolExpr> cond_;
  std::unique_ptr<Stmt> body_;
};

/**
 * @brief 表示For语句
 * @anchor ForStmt
 * @see RepetitiveStmt
 * @note for_statement -> \n 
 *        FOR ID ASSIGN expr TO expr DO statement \n
 *        | FOR ID ASSIGN expr DOWNTO expr DO statement
 */
class ForStmt: public RepetitiveStmt
{
public:
  ForStmt(
      std::unique_ptr<Expr> ctrl_var,
      std::unique_ptr<Expr> init_val,
      std::unique_ptr<Expr> cond,
      std::unique_ptr<Stmt> body,
      bool updown
  )
    : ctrl_var_(std::move(ctrl_var))
    , init_val_(std::move(init_val))
    , end_val_(std::move(cond))
    , body_(std::move(body))
    , updown_(updown)
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto ctrlVar() -> Expr & { return *ctrl_var_; }

  [[nodiscard]] auto initVal() -> Expr & { return *init_val_; }

  [[nodiscard]] auto endVal() -> Expr & { return *end_val_; }

  [[nodiscard]] auto body() -> Stmt & { return *body_; }

  [[nodiscard]] auto updown() -> bool & { return updown_; }

private:
  std::unique_ptr<Expr> ctrl_var_;
  std::unique_ptr<Expr> init_val_;
  std::unique_ptr<Expr> end_val_;
  std::unique_ptr<Stmt> body_;
  bool updown_;
};

/**
 * @brief 表示以 begin 与 end 包围的复合语句
 * @anchor CompoundStmt
 * @see StructuredStmt
 * @note compound_statement -> BEGIN statement_list END
 */
class CompoundStmt: public StructuredStmt
{
public:
  /**
   * @brief 构造一个新的 `CompoundStmt` 对象
   * 
   * @param stmts `std::vector<std::unique_ptr<Stmt>>` 多个语句
   */
  explicit CompoundStmt(std::vector<std::unique_ptr<Stmt>> stmts)
    : stmts_(std::move(stmts))
  {}

  /**
   * @brief 移动构造一个新的 `CompoundStmt` 对象
   */
  CompoundStmt(CompoundStmt &&) = default;

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  /**
   * @brief statement list
   * 
   * @return `const std::vector<std::unique_ptr<Stmt>>&` 语句列表
   */
  [[nodiscard]] auto stmts() const -> const std::vector<std::unique_ptr<Stmt>> & { return stmts_; }

private:
  std::vector<std::unique_ptr<Stmt>> stmts_;  ///< 语句列表
};

/**
 * @brief 表示语句部分
 * @anchor StmtPart
 * @see CompoudStmt
 * @see Block
 * @note statement_part -> compound_statement
 */
class StmtPart: public CompoundStmt
{
public:
  /**
   * @brief 
   * 
   * @param stmts 
   */
  explicit StmtPart(CompoundStmt stmt)
    : CompoundStmt(std::move(stmt))
  {}

  explicit StmtPart(std::unique_ptr<CompoundStmt> stmt)
    : CompoundStmt(std::move(*stmt))
  {}

  StmtPart(StmtPart &&) = default;

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;
};

//*******************************************************
//********************** Program ************************
//*******************************************************

/**
 * @brief 表示程序块
 * @anchor ProgramBlock
 * @see Block
 * @note program_block -> block PERIOD
 */
class ProgramBlock: public Block
{
public:
  /**
   * @brief 构建新的 `ProgramBlock` 对象
   * 
   * @param block `Block` 传递给基类进行初始化
   */
  explicit ProgramBlock(Block block)
    : Block(std::move(block))
  {}

  /**
   * @brief 构建新的 `ProgramBlock` 对象
   * 
   * @param block `Block` 传递给基类进行初始化
   */
  explicit ProgramBlock(std::unique_ptr<Block> block)
    : Block(std::move(*block))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;
};

/**
 * @brief 表示程序的头部信息
 * @anchor ProgramHead
 * @note program_head -> \n 
 *       PROGRAM ID LPAREN id_list RPAREN SEMICOLON
 *     | PROGRAM ID LPAREN RPAREN SEMICOLON
 *     | PROGRAM ID SEMICOLON
 */
class ProgramHead: public ASTNode
{
public:
  /**
   * @brief program 构造函数
   * 
   * @param program_name 程序的名字
   */
  explicit ProgramHead(std::string program_name)
    : program_name_(std::move(program_name))
  {}

  /**
   * @brief program 构造函数
   * 
   * @param program_name 程序的名字
   * @param id_list 程序头部的参数列表
   */
  ProgramHead(
      std::string program_name,
      std::vector<std::string> id_list
  )
    : program_name_(std::move(program_name))
    , id_list_(std::move(id_list))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  /**
   * @brief 返回 program name
   * 
   * @return program name
   */
  [[nodiscard]] auto programName() -> std::string & { return program_name_; }

  /**
   * @brief 程序头部的参数列表
   * 
   * @return std::vector<std::string>& 程序头部的参数列表
   */
  [[nodiscard]] auto idList() -> std::vector<std::string> & { return id_list_; }

private:
  std::string program_name_;          ///< 程序的名字
  std::vector<std::string> id_list_;  ///< 程序头部的参数列表
};

/**
 * @brief 表示一个Pascal程序，Pascal程序分为 head 和 block 两部分
 * @anchor Program
 * @see ProgramHead ProgramBlock
 * @note program -> program_head program_block
 */
class Program: public ASTNode
{
public:
  /**
   * @brief Program 构造函数
   * 
   * @param head program head.
   * @param block program block.
   */
  Program(
      std::unique_ptr<ProgramHead> head,
      std::unique_ptr<ProgramBlock> block
  )
    : head_(std::move(head))
    , block_(std::move(block))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  /**
   * 返回 `ProgramHead` 对象引用
   *
   * @return `ProgramHead` 对象引用.
   */
  [[nodiscard]] auto head() -> ProgramHead & { return *head_; }

  /**
   * 返回 `ProgramBlock` 对象引用
   *
   * @return `ProgramBlock` 对象引用
   */
  [[nodiscard]] auto block() -> ProgramBlock & { return *block_; }

private:
  std::unique_ptr<ProgramHead> head_;    ///< The program head.
  std::unique_ptr<ProgramBlock> block_;  ///< The program block.
};

}  // namespace ast

}  // namespace pascc
