#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "location.hh"

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
   * @attention 如果 stmt_part 为空，那么源程序一定是错误的
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
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;
};

/**
 * @brief 表示布尔表达式
 * @anchor BoolExpr
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
  {}

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
  {}

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
  {}

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
 * @brief number 类
 * @anchor Number
 * @note num -> INT_NUM | REAL_NUM                    
 */
class Number: public ASTNode
{
public:
  explicit Number(int value)
    : type_("integer")
    , value_(value)
  {}

  explicit Number(double value)
    : type_("real")
    , value_(value)
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto type() -> std::string & { return type_; }

  [[nodiscard]] auto value() -> std::variant<int, double> { return value_; }

private:
  std::string type_;                 ///< real 或 integer
  std::variant<int, double> value_;  ///< 使用 std::get<int>(value_) 或 std::get<double>(value_) 获取值
};

/**
 * @brief 表示字符串字面量
 * @anchor StringLiteral
 */
class StringLiteral: public Expr
{
public:
  explicit StringLiteral(std::string string)
    : value_(std::move(string))
  {}

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
 */
class UnsignedConstant: public Expr
{
public:
  explicit UnsignedConstant(std::unique_ptr<Number> number)
  {
    if (number->type() == "integer") {
      type_  = "integer";
      value_ = std::get<int>(number->value());
    } else {
      type_  = "real";
      value_ = std::get<double>(number->value());
    }
  }

  explicit UnsignedConstant(char value)
    : type_("char")
    , value_(value)
  {}

  explicit UnsignedConstant(bool value)
    : type_("boolean")
    , value_(value)
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto type() -> std::string & { return type_; }

  [[nodiscard]] auto value() -> std::variant<int, double, char, bool> { return value_; }

private:
  std::string type_;
  std::variant<int, double, char, bool> value_;
};

/**
 * @brief 表示函数调用
 * @anchor FuncCall
 */
class FuncCall: public Expr
{
public:
  FuncCall(std::string funcid, std::vector<std::unique_ptr<Expr>> actuals)
    : funcid_(std::move(funcid))
    , actuals_(std::move(actuals))
  {}

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
 */
class Assignable: public Expr
{
public:
  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;
};

/**
 * @brief 表示一个可以赋值的标识符，如变量名与函数名
 * @anchor AssignableId
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
 * @brief 索引变量，如 a[10]
 * @anchor IndexedVar
 */
class IndexedVar: public Assignable
{
public:
  IndexedVar(
      std::unique_ptr<Assignable> assignable,
      std::vector<std::unique_ptr<Expr>> indices
  )
    : assignable_(std::move(assignable))
    , indices_(std::move(indices))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto assignable() -> Assignable & { return *assignable_; }

  [[nodiscard]] auto indices() -> std::vector<std::unique_ptr<Expr>> & { return indices_; }

private:
  std::unique_ptr<Assignable> assignable_;
  std::vector<std::unique_ptr<Expr>> indices_;
};

/**
 * @brief 结构体成员访问，point.x
 * @anchor FieldDesignator
 */
class FieldDesignator: public Assignable
{
public:
  FieldDesignator(
      std::unique_ptr<Assignable> assignable,
      std::string field
  )
    : assignable_(std::move(assignable))
    , field_(std::move(field))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto assignable() -> Assignable & { return *assignable_; }

  [[nodiscard]] auto field() -> std::string & { return field_; }

private:
  std::unique_ptr<Assignable> assignable_;
  std::string field_;
};


/**
 * @brief 表示常量类
 * @anchor Constant
 * @note constant: \n
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
    , type_(number->type())
  {
    if (number->type() == "integer") {
      value_ = std::get<int>(number->value());
    } else {
      value_ = std::get<double>(number->value());
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
 * @brief 单独常量声明
 * @anchor ConstDecl
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
 * @brief 常量声明部分
 * @anchor ConstDeclPart
 * @see Block
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
 * @brief 类型表示符
 * @anchor TypeDenoter
 * @see TypeId ArrayType RecordType
 */
class TypeDenoter: public ASTNode
{
public:
  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;
};

/**
 * @brief 类型标识符，基本类型，以及用户声明声明的类型
 * @anchor TypeId
 * @see TypeDenoter
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
 * @brief 数组类型
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
    : type_(std::move(type))
    , periods_(std::move(periods))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto type() -> TypeDenoter & { return *type_; }

  [[nodiscard]] auto periods() -> std::vector<std::unique_ptr<Period>> & { return periods_; }

private:
  std::unique_ptr<TypeDenoter> type_;
  std::vector<std::unique_ptr<Period>> periods_;
};

/**
 * @brief 记录类型，与C语言的结构体类似
 * @anchor RecordType
 * @see TypeDenoter
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
 * @brief 类型声明
 * @anchor TypeDecl
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
 * @brief 变量声明部分
 * @anchor TypeDeclPart
 * @see Block
 * @note type_declaration_part -> ε | TYPE type_declarations SEMICOLON \n
 * type_declarations -> type_declarations SEMICOLON ID EQ type_denoter \n
 *                    | ID EQ type_denoter
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
 * @brief 单个变量声明
 * @anchor VarDecl
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

private:
  std::vector<std::string> id_list_;
  std::unique_ptr<TypeDenoter> type_;
};

/**
 * @brief 变量声明部分
 * @anchor VarDeclPart
 * @see Block
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
 * @brief 单个子程序声明
 * @anchor SubprogDecl
 */
class SubprogDecl: public ASTNode
{
public:
  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;
};

/**
 * @brief 形式参数
 * @anchor FormalParam
 * @see ValueParamSpec VarParamSpec
 */
class FormalParam: public ASTNode
{
public:
  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;
};

/**
 * @brief 传值形式参数传递
 * @anchor ValueParamSpec
 * @see FormalParam
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
 * @brief 引用形式参数传递
 * @anchor VarParamSpec
 * @see FormalParam
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

private:
  std::vector<std::string> id_list_;
  std::unique_ptr<TypeDenoter> type_;
};

/**
 * @brief 过程声明的头部信息
 * @anchor ProcHead
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

private:
  std::string proc_id_;
  std::vector<std::unique_ptr<FormalParam>> formal_params_;
};

/**
 * @brief 过程块
 * @anchor ProcBlock
 * @see Block
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
 * @brief 单个过程声明
 * @anchor ProcDecl
 * @see SubprogDecl
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
 * @brief 函数头部信息
 * @anchor FuncHead
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

private:
  std::string func_id_;
  std::vector<std::unique_ptr<FormalParam>> formal_params_;
  std::unique_ptr<TypeDenoter> return_type_;
};

/**
 * @brief 函数块
 * @anchor FuncBlock
 * @see Block
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
 * @brief 单个函数声明
 * @anchor FuncDecl
 * @see SubprogDecl
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
 * @brief 子程序声明部分
 * @anchor SubprogDeclPart
 * @see Block
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
 */
class Stmt: public ASTNode
{
public:
  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;
};

/**
 * @brief 表示一个简单语句
 * @anchor SimpleStmt
 * @see AssignStmt ProcCallStmt
 */
class SimpleStmt: public Stmt
{
public:
  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;
};

/**
 * @brief 表示一个赋值语句
 * @anchor AssignStmt
 * @see SimpleStmt
 */
class AssignStmt: public SimpleStmt
{
public:
  AssignStmt(
      std::unique_ptr<Assignable> lhs,
      std::unique_ptr<Expr> rhs
  )
    : lhs_(std::move(lhs))
    , rhs_(std::move(rhs))
  {}

  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;

  [[nodiscard]] auto lhs() -> Assignable & { return *lhs_; }

  [[nodiscard]] auto rhs() -> Expr & { return *rhs_; }

private:
  std::unique_ptr<Assignable> lhs_;
  std::unique_ptr<Expr> rhs_;
};

/**
 * @brief 表示一个过程调用语句
 * @anchor ProcCallStmt
 * @see SimpleStmt
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
 * @brief 特殊过程调用 Read
 * @anchor ReadStmt
 * @see ProcCallStmt
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
 * @brief 特殊过程调用 Write
 * @anchor WriteStmt
 * @see ProcCallStmt
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
 * @brief 特殊过程调用 Readln
 * @anchor ReadlnStmt
 * @see ProcCallStmt
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
 * @brief 特殊过程调用 Writeln
 * @anchor WritelnStmt
 * @see ProcCallStmt
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
 * @brief 特殊过程调用 Exit
 * @anchor ExitStmt
 * @see ProcCallStmt
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
 * @brief 结构化语句
 * @anchor StructuredStmt
 * @see ConditionalStmt RepetitiveStmt
 */
class StructuredStmt: public Stmt
{
public:
  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;
};

/**
 * @brief 条件语句
 * @anchor ConditionalStmt
 * @see StructuredStmt
 * @see IfStmt CaseStmt
 */
class ConditionalStmt: public StructuredStmt
{
public:
  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;
};

/**
 * @brief If语句
 * @anchor IfStmt
 * @see ConditionalStmt
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
 */
class RepetitiveStmt: public StructuredStmt
{
public:
  /**
   * @ref accept "ASTNode::accept"
   */
  void accept(Visitor &v) override;
};

/**
 * @brief Repeat语句
 * @anchor RepeatStmt
 * @see RepetitiveStmt
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
 * @brief While语句
 * @anchor WhileStmt
 * @see RepetitiveStmt
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
 * @brief For语句
 * @anchor ForStmt
 * @see RepetitiveStmt
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
 * @brief statement_part
 * @anchor StmtPart
 * @see CompoudStmt
 * @see Block
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
 * @brief 表示 program block
 * @anchor ProgramBlock
 * @see Block
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
 * @brief Program 的头部信息
 * @anchor ProgramHead
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
 * @brief Pascal程序分为 head 和 block 两部分
 * @anchor Program
 * @see ProgramHead ProgramBlock
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


/**
 * @brief Visitor 抽象基类
 *
 * @b 访问者设计模式：
 * 
 *   <a href="https://www.youtube.com/watch?v=PEcy1vYHb8A">视频讲解</a> \n
 *   <a href="https://en.wikipedia.org/wiki/Visitor_pattern">维基百科</a> \n
 *   <a href="https://refactoringguru.cn/design-patterns/visitor">设计模式</a> \n
 *   <a href="https://zhuanlan.zhihu.com/p/380161731">知乎链接</a> \n
 */
class Visitor
{
public:
  virtual ~Visitor()                           = default;

  virtual void visit(ast::Block &node)         = 0;
  virtual void visit(ast::Number &node)        = 0;
  virtual void visit(ast::Constant &node)      = 0;
  virtual void visit(ast::StringLiteral &node) = 0;


  /// expression
  virtual void visit(ast::Expr &node)             = 0;
  virtual void visit(ast::UnsignedConstant &node) = 0;
  virtual void visit(ast::BinaryExpr &node)       = 0;
  virtual void visit(ast::UnaryExpr &node)        = 0;
  virtual void visit(ast::FuncCall &node)         = 0;
  virtual void visit(ast::Assignable &node)       = 0;
  virtual void visit(ast::AssignableId &node)     = 0;
  virtual void visit(ast::IndexedVar &node)       = 0;
  virtual void visit(ast::FieldDesignator &node)  = 0;


  /// const declaration
  virtual void visit(ast::ConstDecl &node)     = 0;
  virtual void visit(ast::ConstDeclPart &node) = 0;


  /// type declaration
  virtual void visit(ast::TypeId &node)       = 0;
  virtual void visit(ast::Period &node)       = 0;
  virtual void visit(ast::ArrayType &node)    = 0;
  virtual void visit(ast::RecordType &node)   = 0;
  virtual void visit(ast::TypeDecl &node)     = 0;
  virtual void visit(ast::TypeDeclPart &node) = 0;
  virtual void visit(ast::TypeDenoter &node)  = 0;


  /// var declaration
  virtual void visit(ast::VarDeclPart &node)    = 0;
  virtual void visit(ast::ValueParamSpec &node) = 0;
  virtual void visit(ast::VarParamSpec &node)   = 0;
  virtual void visit(ast::VarDecl &node)        = 0;


  /// subprogram declaration
  virtual void visit(ast::ProcHead &node)        = 0;
  virtual void visit(ast::ProcBlock &node)       = 0;
  virtual void visit(ast::ProcDecl &node)        = 0;
  virtual void visit(ast::FuncHead &node)        = 0;
  virtual void visit(ast::FuncBlock &node)       = 0;
  virtual void visit(ast::FuncDecl &node)        = 0;
  virtual void visit(ast::FormalParam &node)     = 0;
  virtual void visit(ast::SubprogDecl &node)     = 0;
  virtual void visit(ast::SubprogDeclPart &node) = 0;


  /// statement
  virtual void visit(ast::Stmt &node) = 0;
  // conditional statement
  virtual void visit(ast::IfStmt &node)          = 0;
  virtual void visit(ast::CaseStmt &node)        = 0;
  virtual void visit(ast::CaseListElement &node) = 0;
  // repetitive statement
  virtual void visit(ast::RepeatStmt &node) = 0;
  virtual void visit(ast::WhileStmt &node)  = 0;
  virtual void visit(ast::ForStmt &node)    = 0;
  // simple statement
  virtual void visit(ast::AssignStmt &node)   = 0;
  virtual void visit(ast::ProcCallStmt &node) = 0;
  virtual void visit(ast::ReadStmt &node)     = 0;
  virtual void visit(ast::WriteStmt &node)    = 0;
  virtual void visit(ast::ReadlnStmt &node)   = 0;
  virtual void visit(ast::WritelnStmt &node)  = 0;
  virtual void visit(ast::ExitStmt &node)     = 0;
  virtual void visit(ast::CompoundStmt &node) = 0;
  virtual void visit(ast::StmtPart &node)     = 0;

  /// program
  virtual void visit(ast::ProgramBlock &node) = 0;
  virtual void visit(ast::ProgramHead &node)  = 0;
  virtual void visit(ast::Program &node)      = 0;
};


}  // namespace pascc
