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
 * @brief 表示 Abstract Syntax Tree (AST) 的节点
 */
class ASTNode
{
public:
  virtual ~ASTNode()              = default;

  virtual void accept(Visitor &v) = 0;

  auto location() -> parse::location & { return loc_; }

private:
  parse::location loc_;
};

/**
 * @brief Block 基类
 *  ProgramBlock -> Block
 *  FuncBlock -> Block
 *  ProcBlock -> Block
 */
class Block: public ASTNode
{
public:
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

  Block(Block &&) = default;

  void accept(Visitor &v) override;

  [[nodiscard]] auto hasConstDeclPart() -> bool { return const_decl_part_ != nullptr; }

  [[nodiscard]] auto hasTypeDeclPart() -> bool { return type_decl_part_ != nullptr; }

  [[nodiscard]] auto hasVarDeclPart() -> bool { return var_decl_part_ != nullptr; }

  [[nodiscard]] auto hasSubprogDeclPart() -> bool { return subprog_decl_part_ != nullptr; }

  [[nodiscard]] auto hasStmtPart() -> bool { return stmt_part_ != nullptr; }

  [[nodiscard]] auto constDeclPart() -> ConstDeclPart & { return *const_decl_part_; }

  [[nodiscard]] auto typeDeclPart() -> TypeDeclPart & { return *type_decl_part_; }

  [[nodiscard]] auto varDeclPart() -> VarDeclPart & { return *var_decl_part_; }

  [[nodiscard]] auto subprogDeclPart() -> SubprogDeclPart & { return *subprog_decl_part_; }

  [[nodiscard]] auto stmtPart() -> StmtPart & { return *stmt_part_; }

private:
  std::unique_ptr<ConstDeclPart> const_decl_part_;
  std::unique_ptr<TypeDeclPart> type_decl_part_;
  std::unique_ptr<VarDeclPart> var_decl_part_;
  std::unique_ptr<SubprogDeclPart> subprog_decl_part_;
  std::unique_ptr<StmtPart> stmt_part_;
};

/**
 * @brief 表示 expr 基类
 */
class Expr: public ASTNode
{
public:
  void accept(Visitor &v) override;
};

/**
 * @brief 表示布尔表达式
 */
class BoolExpr: public Expr
{
public:
  explicit BoolExpr(std::unique_ptr<Expr> expr)
    : expr_(std::move(expr))
  {}

  void accept(Visitor &v) override;

  [[nodiscard]] auto expr() -> Expr & { return *expr_; }

private:
  std::unique_ptr<Expr> expr_;
};

enum class BinOp
{
  PLUS,   ///< +
  MINUS,  ///< -
  MUL,    ///< *
  FDIV,   ///< /
  IDIV,   ///< div
  MOD,    ///< mod
  AND,    ///< and
  OR,     ///< or
  EQ,     ///< =
  NE,     ///< <>
  LT,     ///< <
  GT,     ///< >
  LE,     ///< <=
  GE      ///< >=
};

auto operator<<(std::ostream &os, BinOp op) -> std::ostream &;

/**
 * @brief 表示 binary expression.
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
   * Accepts a visitor and calls the appropriate visit method.
   * @param v The visitor object.
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
  NOT,    ///< not
  MINUS,  ///< -
  PLUS    ///< +
};

auto operator<<(std::ostream &os, UnaryOp op) -> std::ostream &;

/**
 * 表示 unary expression.
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
   * Accepts a visitor and invokes the appropriate visit method.
   * @param v The visitor object.
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
 *
 *  num -> INT_NUM
 *       | REAL_NUM                    
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

  void accept(Visitor &v) override;

  [[nodiscard]] auto type() -> std::string & { return type_; }

  [[nodiscard]] auto value() -> std::variant<int, double> { return value_; }

private:
  std::string type_;                 ///< real 或 integer
  std::variant<int, double> value_;  ///< 使用 std::get<int>(value_) 或 std::get<double>(value_) 获取值
};

class StringLiteral: public Expr
{
public:
  explicit StringLiteral(std::string string)
    : value_(std::move(string))
  {}

  void accept(Visitor &v) override;

  [[nodiscard]] auto value() -> std::string & { return value_; }

private:
  std::string value_;
};

/**
 * @brief 表示 unsigned constant expression.
 * 
 * This class is a subclass of the `Expr` class and represents an unsigned constant value.
 * It stores the value of the constant as an unsigned integer.
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

  void accept(Visitor &v) override;

  [[nodiscard]] auto type() -> std::string & { return type_; }

  [[nodiscard]] auto value() -> std::variant<int, double, char, bool> { return value_; }

private:
  std::string type_;
  std::variant<int, double, char, bool> value_;
};

/**
 * Represents a function call expression.
 */
class FuncCall: public Expr
{
public:
  FuncCall(std::string funcid, std::vector<std::unique_ptr<Expr>> actuals)
    : funcid_(std::move(funcid))
    , actuals_(std::move(actuals))
  {}

  void accept(Visitor &v) override;

  [[nodiscard]] auto funcid() -> std::string & { return funcid_; }

  [[nodiscard]] auto actuals() -> std::vector<std::unique_ptr<Expr>> & { return actuals_; }

private:
  std::string funcid_;
  std::vector<std::unique_ptr<Expr>> actuals_;
};

/**
 * @brief Represents a variable access expression.
 * 
 * This class is a subclass of the `Expr` class and represents an expression that accesses a variable.
 * It provides a method `accept` for visitor pattern implementation.
 */
class Assignable: public Expr
{
public:
  void accept(Visitor &v) override;
};

/**
 * Represents an entire variable access in the abstract syntax tree.
 * This class is derived from the VariableAccess class.
 */
class AssignableId: public Assignable
{
public:
  explicit AssignableId(std::string id)
    : id_(std::move(id))
  {}

  void accept(Visitor &v) override;

  [[nodiscard]] auto id() -> std::string & { return id_; }

private:
  std::string id_;
};

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

  void accept(Visitor &v) override;

  [[nodiscard]] auto assignable() -> Assignable & { return *assignable_; }

  [[nodiscard]] auto indices() -> std::vector<std::unique_ptr<Expr>> & { return indices_; }

private:
  std::unique_ptr<Assignable> assignable_;
  std::vector<std::unique_ptr<Expr>> indices_;
};

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

  void accept(Visitor &v) override;

  [[nodiscard]] auto assignable() -> Assignable & { return *assignable_; }

  [[nodiscard]] auto field() -> std::string & { return field_; }

private:
  std::unique_ptr<Assignable> assignable_;
  std::string field_;
};


/**
 * @brief 常量 
 *  constant -> PLUS I
 *            | MINUS ID
 *            | ID
 *            | num
 *            | PLUS num
 *            | MINUS num
 *            | CHAR
 *            | string_literal
 */
class Constant: public ASTNode
{
public:
  explicit Constant(std::string id, int sign = 1)
    : sign_(sign)
    , type_("reference")
    , value_(std::move(id))
  {}

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

  explicit Constant(char value, int sign = 1)
    : sign_(sign)
    , type_("char")
    , value_(value)
  {}

  explicit Constant(std::unique_ptr<StringLiteral> string_literal)
    : sign_(1)
    , type_("string")
    , value_(string_literal->value())
  {}

  void accept(Visitor &v) override;

  [[nodiscard]] auto sign() -> int & { return sign_; }

  [[nodiscard]] auto type() -> std::string & { return type_; }

  [[nodiscard]] auto value() -> std::variant<std::string, int, double, char> { return value_; }

private:
  int sign_;
  std::string type_;
  std::variant<std::string, int, double, char> value_;
};

class ConstDecl: public ASTNode
{
public:
  ConstDecl(std::string const_id, std::unique_ptr<Constant> constant)
    : const_id_(std::move(const_id))
    , constant_(std::move(constant))
  {}

  void accept(Visitor &v) override;

  [[nodiscard]] auto constId() -> std::string & { return const_id_; }

  [[nodiscard]] auto constant() -> Constant & { return *constant_; }

private:
  std::string const_id_;
  std::unique_ptr<Constant> constant_;
};

class ConstDeclPart: public ASTNode
{
public:
  explicit ConstDeclPart(std::vector<std::unique_ptr<ConstDecl>> const_decls)
    : const_decls_(std::move(const_decls))
  {}

  void accept(Visitor &v) override;

  [[nodiscard]] auto constDecls() -> std::vector<std::unique_ptr<ConstDecl>> & { return const_decls_; }

private:
  std::vector<std::unique_ptr<ConstDecl>> const_decls_;
};

//*******************************************************
//************************* Type ************************
//*******************************************************

class TypeDenoter: public ASTNode
{
public:
  void accept(Visitor &v) override;
};

class TypeId: public TypeDenoter
{
public:
  explicit TypeId(std::string id)
    : id_(std::move(id))
  {}

  void accept(Visitor &v) override;

  [[nodiscard]] auto id() -> std::string & { return id_; }

private:
  std::string id_;
};

/**
 * @brief 范围
 * 
 * period -> constant RANGE constant
 */
class Period: public ASTNode
{
public:
  Period(std::unique_ptr<Constant> low, std::unique_ptr<Constant> high)
    : low_(std::move(low))
    , high_(std::move(high))
  {}

  void accept(Visitor &v) override;

  [[nodiscard]] auto low() -> Constant & { return *low_; }

  [[nodiscard]] auto high() -> Constant & { return *high_; }

private:
  std::unique_ptr<Constant> low_;
  std::unique_ptr<Constant> high_;
};

/**
 * @brief type_denoter -> ARRAY LSB periods RSB OF type_denoter
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

  void accept(Visitor &v) override;

  [[nodiscard]] auto type() -> TypeDenoter & { return *type_; }

  [[nodiscard]] auto periods() -> std::vector<std::unique_ptr<Period>> & { return periods_; }

private:
  std::unique_ptr<TypeDenoter> type_;
  std::vector<std::unique_ptr<Period>> periods_;
};

class RecordType: public TypeDenoter
{
public:
  explicit RecordType(std::vector<std::unique_ptr<VarDecl>> fields)
    : fields_(std::move(fields))
  {}

  void accept(Visitor &v) override;

  [[nodiscard]] auto fields() -> std::vector<std::unique_ptr<VarDecl>> & { return fields_; }

private:
  std::vector<std::unique_ptr<VarDecl>> fields_;
};

class TypeDecl: public ASTNode
{
public:
  TypeDecl(std::string type_id, std::unique_ptr<TypeDenoter> type_denoter)
    : type_id_(std::move(type_id))
    , type_denoter_(std::move(type_denoter))
  {}

  void accept(Visitor &v) override;

  [[nodiscard]] auto typeId() -> std::string & { return type_id_; }

  [[nodiscard]] auto typeDenoter() -> TypeDenoter & { return *type_denoter_; }

private:
  std::string type_id_;
  std::unique_ptr<TypeDenoter> type_denoter_;
};

/**
 * @brief 变量声明部分
 *
 * type_declaration_part -> ε | TYPE type_declarations SEMICOLON
 * type_declarations -> type_declarations SEMICOLON ID EQ type_denoter
 *                    | ID EQ type_denoter
 */
class TypeDeclPart: public ASTNode
{
public:
  explicit TypeDeclPart(std::vector<std::unique_ptr<TypeDecl>> type_decls)
    : type_decls_(std::move(type_decls))
  {}

  void accept(Visitor &v) override;

  [[nodiscard]] auto typeDecls() -> std::vector<std::unique_ptr<TypeDecl>> & { return type_decls_; }

private:
  std::vector<std::unique_ptr<TypeDecl>> type_decls_;
};

//*******************************************************
//************************* Var *************************
//*******************************************************

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

  void accept(Visitor &v) override;

  [[nodiscard]] auto idList() -> std::vector<std::string> & { return id_list_; }

  [[nodiscard]] auto type() -> TypeDenoter & { return *type_; }

private:
  std::vector<std::string> id_list_;
  std::unique_ptr<TypeDenoter> type_;
};

class VarDeclPart: public ASTNode
{
public:
  explicit VarDeclPart(std::vector<std::unique_ptr<VarDecl>> var_decls)
    : var_decls_(std::move(var_decls))
  {}

  void accept(Visitor &v) override;

  [[nodiscard]] auto varDecls() -> std::vector<std::unique_ptr<VarDecl>> & { return var_decls_; }

private:
  std::vector<std::unique_ptr<VarDecl>> var_decls_;
};

//*******************************************************
//********************** Subprogram *********************
//*******************************************************

class SubprogDecl: public ASTNode
{
public:
  void accept(Visitor &v) override;
};

class FormalParam: public ASTNode
{
public:
  void accept(Visitor &v) override;
};

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

  void accept(Visitor &v) override;

  [[nodiscard]] auto idList() -> std::vector<std::string> & { return id_list_; }

  [[nodiscard]] auto type() -> TypeDenoter & { return *type_; }

private:
  std::vector<std::string> id_list_;
  std::unique_ptr<TypeDenoter> type_;
};

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

  void accept(Visitor &v) override;

  [[nodiscard]] auto idList() -> std::vector<std::string> & { return id_list_; }

  [[nodiscard]] auto type() -> TypeDenoter & { return *type_; }

private:
  std::vector<std::string> id_list_;
  std::unique_ptr<TypeDenoter> type_;
};

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

  void accept(Visitor &v) override;

  [[nodiscard]] auto procId() -> std::string & { return proc_id_; }

  [[nodiscard]] auto formalParams() -> std::vector<std::unique_ptr<FormalParam>> & { return formal_params_; }

private:
  std::string proc_id_;
  std::vector<std::unique_ptr<FormalParam>> formal_params_;
};

class ProcBlock: public Block
{
public:
  explicit ProcBlock(Block block)
    : Block(std::move(block))
  {}

  explicit ProcBlock(std::unique_ptr<Block> block)
    : Block(std::move(*block))
  {}

  void accept(Visitor &v) override;
};

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

  void accept(Visitor &v) override;

  [[nodiscard]] auto head() -> ProcHead & { return *head_; }

  [[nodiscard]] auto block() -> ProcBlock & { return *block_; }

private:
  std::unique_ptr<ProcHead> head_;
  std::unique_ptr<ProcBlock> block_;
};

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

  void accept(Visitor &v) override;

  [[nodiscard]] auto funcId() -> std::string & { return func_id_; }

  [[nodiscard]] auto formalParams() -> std::vector<std::unique_ptr<FormalParam>> & { return formal_params_; }

  [[nodiscard]] auto returnType() -> TypeDenoter & { return *return_type_; }

private:
  std::string func_id_;
  std::vector<std::unique_ptr<FormalParam>> formal_params_;
  std::unique_ptr<TypeDenoter> return_type_;
};

class FuncBlock: public Block
{
public:
  explicit FuncBlock(Block block)
    : Block(std::move(block))
  {}

  explicit FuncBlock(std::unique_ptr<Block> block)
    : Block(std::move(*block))
  {}

  void accept(Visitor &v) override;
};

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

  void accept(Visitor &v) override;

  [[nodiscard]] auto head() -> FuncHead & { return *head_; }

  [[nodiscard]] auto block() -> FuncBlock & { return *block_; }

private:
  std::unique_ptr<FuncHead> head_;
  std::unique_ptr<FuncBlock> block_;
};

class SubprogDeclPart: public ASTNode
{
public:
  explicit SubprogDeclPart(
      std::vector<std::unique_ptr<SubprogDecl>> subprog_decls
  )
    : subprog_decls_(std::move(subprog_decls))
  {}
  void accept(Visitor &v) override;

  [[nodiscard]] auto subprogDecls() -> std::vector<std::unique_ptr<SubprogDecl>> & { return subprog_decls_; }

private:
  std::vector<std::unique_ptr<SubprogDecl>> subprog_decls_;
};

//*******************************************************
//********************** Statement **********************
//*******************************************************

class Stmt: public ASTNode
{
public:
  void accept(Visitor &v) override;
};

class SimpleStmt: public Stmt
{
public:
  void accept(Visitor &v) override;
};

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

  void accept(Visitor &v) override;

  [[nodiscard]] auto lhs() -> Assignable & { return *lhs_; }

  [[nodiscard]] auto rhs() -> Expr & { return *rhs_; }

private:
  std::unique_ptr<Assignable> lhs_;
  std::unique_ptr<Expr> rhs_;
};

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

  void accept(Visitor &v) override;

  [[nodiscard]] auto procId() -> std::string & { return proc_id_; }

  [[nodiscard]] auto actuals() -> std::vector<std::unique_ptr<Expr>> & { return actuals_; }

private:
  std::string proc_id_;
  std::vector<std::unique_ptr<Expr>> actuals_;
};

class ReadStmt: public ProcCallStmt
{
public:
  ReadStmt()
    : ProcCallStmt("read")
  {}

  explicit ReadStmt(std::vector<std::unique_ptr<Expr>> assignables)
    : ProcCallStmt("read", std::move(assignables))
  {}

  void accept(Visitor &v) override;
};

class WriteStmt: public ProcCallStmt
{
public:
  WriteStmt()
    : ProcCallStmt("write")
  {}

  explicit WriteStmt(std::vector<std::unique_ptr<Expr>> exprs)
    : ProcCallStmt("write", std::move(exprs))
  {}

  void accept(Visitor &v) override;
};

class ReadlnStmt: public ProcCallStmt
{
public:
  ReadlnStmt()
    : ProcCallStmt("readln")
  {}

  explicit ReadlnStmt(std::vector<std::unique_ptr<Expr>> assignables)
    : ProcCallStmt("readln", std::move(assignables))
  {}

  void accept(Visitor &v) override;
};

class WritelnStmt: public ProcCallStmt
{
public:
  WritelnStmt()
    : ProcCallStmt("writeln")
  {}

  explicit WritelnStmt(std::vector<std::unique_ptr<Expr>> exprs)
    : ProcCallStmt("writeln", std::move(exprs))
  {}

  void accept(Visitor &v) override;
};

class ExitStmt: public ProcCallStmt
{
public:
  ExitStmt()
    : ProcCallStmt("exit")
  {}

  explicit ExitStmt(std::vector<std::unique_ptr<Expr>> exprs)
    : ProcCallStmt("exit", std::move(exprs))
  {}

  void accept(Visitor &v) override;
};

class StructuredStmt: public Stmt
{
public:
  void accept(Visitor &v) override;
};

class ConditionalStmt: public StructuredStmt
{
public:
  void accept(Visitor &v) override;
};

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

  void accept(Visitor &v) override;

  [[nodiscard]] auto constants() -> std::vector<std::unique_ptr<Constant>> & { return constants_; }

  [[nodiscard]] auto stmt() -> Stmt & { return *stmt_; }

private:
  std::vector<std::unique_ptr<Constant>> constants_;
  std::unique_ptr<Stmt> stmt_;
};

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

  void accept(Visitor &v) override;

  [[nodiscard]] auto expr() -> Expr & { return *expr_; }

  [[nodiscard]] auto caseList() -> std::vector<std::unique_ptr<CaseListElement>> & { return case_list_; }

private:
  std::unique_ptr<Expr> expr_;
  std::vector<std::unique_ptr<CaseListElement>> case_list_;
};

class RepetitiveStmt: public StructuredStmt
{
public:
  void accept(Visitor &v) override;
};

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

  void accept(Visitor &v) override;

  [[nodiscard]] auto body() -> std::vector<std::unique_ptr<Stmt>> & { return body_; }

  [[nodiscard]] auto cond() -> Expr & { return *cond_; }

private:
  std::vector<std::unique_ptr<Stmt>> body_;
  std::unique_ptr<BoolExpr> cond_;
};

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

  void accept(Visitor &v) override;

  [[nodiscard]] auto cond() -> Expr & { return *cond_; }

  [[nodiscard]] auto body() -> Stmt & { return *body_; }

private:
  std::unique_ptr<BoolExpr> cond_;
  std::unique_ptr<Stmt> body_;
};

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

class CompoundStmt: public StructuredStmt
{
public:
  explicit CompoundStmt(std::vector<std::unique_ptr<Stmt>> stmts)
    : stmts_(std::move(stmts))
  {}

  CompoundStmt(CompoundStmt &&) = default;

  void accept(Visitor &v) override;

  [[nodiscard]] auto stmts() const -> const std::vector<std::unique_ptr<Stmt>> & { return stmts_; }

private:
  std::vector<std::unique_ptr<Stmt>> stmts_;
};

/**
 * @brief statement_part
 *
 * statement_part -> compound_statement
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
   * @brief 
   * 
   * @param v 
   */
  void accept(Visitor &v) override;
};

//*******************************************************
//********************** Program ************************
//*******************************************************

/**
 * @brief Represents a program block.
 * 
 * This class inherits from the `Block` class and provides an implementation for the `accept` method.
 * The `accept` method allows a visitor to visit and perform operations on the program block.
 */
class ProgramBlock: public Block
{
public:
  explicit ProgramBlock(Block block)
    : Block(std::move(block))
  {}

  explicit ProgramBlock(std::unique_ptr<Block> block)
    : Block(std::move(*block))
  {}

  void accept(Visitor &v) override;
};

/**
 * @class ProgramHead
 * @brief Program的头部信息
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
   * @brief 访问者设计模式接口
   */
  void accept(Visitor &v) override;

  /**
   * @brief 返回 program name
   * 
   * @return program name
   */
  [[nodiscard]] auto programName() -> std::string & { return program_name_; }

  /**
   * @brief 返回 id list
   * 
   * @return id list
   */
  [[nodiscard]] auto idList() -> std::vector<std::string> & { return id_list_; }

private:
  std::string program_name_;          ///< 程序的名字
  std::vector<std::string> id_list_;  ///< 程序头部的参数列表
};

/**
 * @brief Represents a program in the abstract syntax tree.
 * 
 * This class is derived from the ASTNode class and contains a program head and a program block.
 * It provides methods to access the program head and program block.
 */
class Program: public ASTNode
{
public:
  /**
   * @brief Constructs a Program object with the given program head and program block.
   * 
   * @param head The program head.
   * @param block The program block.
   */
  Program(
      std::unique_ptr<ProgramHead> head,
      std::unique_ptr<ProgramBlock> block
  )
    : head_(std::move(head))
    , block_(std::move(block))
  {}

  void accept(Visitor &v) override;

  [[nodiscard]] auto head() -> ProgramHead & { return *head_; }

  [[nodiscard]] auto block() -> ProgramBlock & { return *block_; }

private:
  std::unique_ptr<ProgramHead> head_;    ///< The program head.
  std::unique_ptr<ProgramBlock> block_;  ///< The program block.
};

}  // namespace ast

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
