#pragma once

#include <string>
#include <utility>
#include <vector>

namespace pascc {

class Visitor;

namespace ast {

/**
 * @brief 
 * This class represents a node in the Abstract Syntax Tree (AST).
 * It provides common functionality and properties for all AST nodes.
 */
class ASTNode
{
public:
  /**
   * @brief Virtual destructor for the ASTNode class.
   * 
   * This destructor is declared as virtual to ensure that the correct destructor
   * is called when deleting an object through a pointer to the base class.
   */
  virtual ~ASTNode() = default;

  /**
   * @brief Accepts a visitor object and invokes the appropriate visit method.
   * 
   * This pure virtual function is used to implement the Visitor pattern.
   * It allows a visitor object to traverse the AST and perform operations on each node.
   * The specific visit method to be called is determined by the dynamic type of the node.
   * 
   * @param v The visitor object to accept.
   */
  virtual void accept(Visitor &v) = 0;

  /**
   * @brief Returns the line number where the node is located in the source code.
   * 
   * @return The line number of the node.
   */
  [[nodiscard]] auto lineno() const -> int { return lineno_; }

  /**
   * @brief Sets the line number where the node is located in the source code.
   * 
   * @param lineno The line number to set.
   */
  void setLineNumber(int lineno) { lineno_ = lineno; }

  /**
   * @brief Returns the column number where the node is located in the source code.
   * 
   * @return The column number of the node.
   */
  [[nodiscard]] auto colno() const -> int { return colno_; }

  /**
   * @brief Sets the column number where the node is located in the source code.
   * 
   * @param colno The column number to set.
   */
  void setColumnNumber(int colno) { colno_ = colno; }

private:
  int lineno_{};
  int colno_{};
};

/**
 * @brief Represents an expression in the abstract syntax tree.
 * 
 * This class is a subclass of ASTNode and provides a base class for all types of expressions.
 * It defines a virtual function accept() that allows a visitor to traverse the expression.
 */
class Expr: public ASTNode
{
public:
  void accept(Visitor &v) override;
};

enum class BinOp
{
  Plus,   ///< +
  Minus,  ///< -
  Mul,    ///< *
  Div,    ///< /
  Mod,    ///< mod
  And,    ///< and
  Or,     ///< or
  Eq,     ///< =
  Neq,    ///< <>
  Lt,     ///< <
  Gt,     ///< >
  Le,     ///< <=
  Ge      ///< >=
};

/**
 * Represents a binary expression.
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
  [[nodiscard]] auto op() const -> BinOp { return binop_; }

  /**
   * Gets the left-hand side expression.
   * @return The left-hand side expression.
   */
  [[nodiscard]] auto lhs() const -> const Expr & { return *lhs_; }

  /**
   * Gets the right-hand side expression.
   * @return The right-hand side expression.
   */
  [[nodiscard]] auto rhs() const -> const Expr & { return *rhs_; }

private:
  BinOp binop_;
  std::unique_ptr<Expr> lhs_;
  std::unique_ptr<Expr> rhs_;
};

/**
 * Represents a logical NOT expression.
 */
class NotExpr: public Expr
{
public:
  /**
   * Constructs a `NotExpr` object with the given expression.
   * @param expr The expression to be negated.
   */
  explicit NotExpr(std::unique_ptr<Expr> expr)
    : expr_(std::move(expr))
  {}

  /**
   * Accepts a visitor and invokes the appropriate visit method.
   * @param v The visitor object.
   */
  void accept(Visitor &v) override;

private:
  std::unique_ptr<Expr> expr_;
};

/**
 * Represents a negation expression.
 * 
 * This class inherits from the Expr class and represents a negation expression.
 * It holds a unique pointer to an Expr object that represents the expression
 * being negated.
 */
class NegExpr: public Expr
{
public:
  /**
   * Constructs a NegExpr object with the given expression.
   * 
   * @param expr A unique pointer to an Expr object representing the expression
   *             being negated.
   */
  explicit NegExpr(std::unique_ptr<Expr> expr)
    : expr_(std::move(expr))
  {}

  /**
   * Accepts a visitor and calls the appropriate visit method.
   * 
   * This method is used to implement the visitor pattern. It accepts a visitor
   * object and calls the appropriate visit method based on the type of the
   * expression being negated.
   * 
   * @param v The visitor object to accept.
   */
  void accept(Visitor &v) override;

private:
  std::unique_ptr<Expr> expr_;
};

/**
 * @brief Represents an unsigned constant expression.
 * 
 * This class is a subclass of the `Expr` class and represents an unsigned constant value.
 * It stores the value of the constant as an unsigned integer.
 */
class UnsignedConstant: public Expr
{
public:
  /**
   * @brief Constructs an `UnsignedConstant` object with the given value.
   * 
   * @param value The value of the unsigned constant.
   */
  explicit UnsignedConstant(unsigned value)
    : value_(value)
  {}

  /**
   * @brief Accepts a visitor and calls the appropriate visit method.
   * 
   * This method is used for visitor pattern implementation.
   * It calls the appropriate visit method of the provided visitor object.
   * 
   * @param v The visitor object to accept.
   */
  void accept(Visitor &v) override;

private:
  unsigned value_; /**< The value of the unsigned constant. */
};

/**
 * Represents a function call expression.
 */
class FuncCall: public Expr
{
public:
  /**
   * Constructs a `FuncCall` object with the given function identifier and actual arguments.
   * 
   * @param funcid The identifier of the function being called.
   * @param actuals The actual arguments passed to the function.
   */
  FuncCall(std::string funcid, std::vector<std::unique_ptr<Expr>> actuals)
    : funcid_(std::move(funcid))
    , actuals_(std::move(actuals))
  {}

  /**
   * Accepts a visitor and invokes the appropriate visit method.
   * 
   * @param v The visitor object.
   */
  void accept(Visitor &v) override;

private:
  std::string funcid_;                          // The identifier of the function being called.
  std::vector<std::unique_ptr<Expr>> actuals_;  // The actual arguments passed to the function.
};

/**
 * @brief Represents a variable access expression.
 * 
 * This class is a subclass of the `Expr` class and represents an expression that accesses a variable.
 * It provides a method `accept` for visitor pattern implementation.
 */
class VariableAccess: public Expr
{
public:
  void accept(Visitor &v) override;
};

/**
 * Represents an entire variable access in the abstract syntax tree.
 * This class is derived from the VariableAccess class.
 */
class EntireVariableAccess: public VariableAccess
{
public:
  /**
   * Constructs an EntireVariableAccess object with the given variable identifier.
   * @param varid The identifier of the variable.
   */
  explicit EntireVariableAccess(std::string varid)
    : varid_(std::move(varid))
  {}

  /**
   * Accepts a visitor and calls the appropriate visit method based on the concrete type of this object.
   * @param v The visitor object.
   */
  void accept(Visitor &v) override;

private:
  std::string varid_; /**< The identifier of the variable. */
};

/**
 * @brief The base class for all statement nodes in the abstract syntax tree (AST).
 * 
 * This class represents a statement in the AST. It is derived from the `ASTNode` class.
 * All statement nodes should inherit from this class.
 */
class Stmt: public ASTNode
{
public:
  void accept(Visitor &v) override;
};

/**
 * Represents an if statement in the AST.
 */
class IfStmt: public Stmt
{
public:
  /**
   * Constructs an IfStmt object with the given condition, then statement, and else statement.
   * @param cond The condition expression of the if statement.
   * @param then The statement to be executed if the condition is true.
   * @param else_ The statement to be executed if the condition is false.
   */
  IfStmt(std::unique_ptr<Expr> cond, std::unique_ptr<Stmt> then, std::unique_ptr<Stmt> else_)
    : cond_(std::move(cond))
    , then_(std::move(then))
    , else_(std::move(else_))
  {}

  /**
   * Accepts a visitor and calls the appropriate visit method based on the type of the statement.
   * @param v The visitor object.
   */
  void accept(Visitor &v) override;

private:
  std::unique_ptr<Expr> cond_;  ///< The condition expression of the if statement.
  std::unique_ptr<Stmt> then_;  ///< The statement to be executed if the condition is true.
  std::unique_ptr<Stmt> else_;  ///< The statement to be executed if the condition is false.
};

/**
 * @brief Represents a while statement in the AST.
 * 
 * This class is a subclass of Stmt and represents a while statement in the abstract syntax tree (AST).
 * It contains a condition expression and a body statement.
 */
class WhileStmt: public Stmt
{
public:
  /**
   * @brief Constructs a WhileStmt object with the given condition and body.
   * 
   * @param cond The condition expression of the while statement.
   * @param body The body statement of the while statement.
   */
  WhileStmt(std::unique_ptr<Expr> cond, std::unique_ptr<Stmt> body)
    : cond_(std::move(cond))
    , body_(std::move(body))
  {}

  /**
   * @brief Accepts a visitor and calls the appropriate visit method.
   * 
   * This method is used to implement the visitor pattern for the while statement.
   * It calls the visit method of the provided visitor object, passing itself as an argument.
   * 
   * @param v The visitor object to accept.
   */
  void accept(Visitor &v) override;

private:
  std::unique_ptr<Expr> cond_;  ///< The condition expression of the while statement.
  std::unique_ptr<Stmt> body_;  ///< The body statement of the while statement.
};


/**
 * @brief Represents a for loop statement.
 * 
 * This class is derived from the Stmt class and represents a for loop statement in the AST.
 * It contains information about the control variable, initial value, condition, body, and direction (to or downto).
 */
class ForLoopStmt: public Stmt
{
public:
  /**
   * @brief Constructs a ForLoopStmt object.
   * 
   * @param ctrl_var The control variable expression.
   * @param init_val The initial value expression.
   * @param cond The condition expression.
   * @param body The body statement.
   * @param to Specifies the direction of the loop (true for 'to', false for 'downto').
   */
  ForLoopStmt(std::unique_ptr<Expr> ctrl_var, std::unique_ptr<Expr> init_val, std::unique_ptr<Expr> cond, std::unique_ptr<Stmt> body, bool to)
    : ctrl_var_(std::move(ctrl_var))
    , init_val_(std::move(init_val))
    , cond_(std::move(cond))
    , body_(std::move(body))
    , to_(to)
  {}

  /**
   * @brief Accepts a visitor and calls the appropriate visit method.
   * 
   * @param v The visitor object.
   */
  void accept(Visitor &v) override;

  /**
   * @brief Returns the control variable expression.
   * 
   * @return The control variable expression.
   */
  [[nodiscard]] auto ctrlVar() const -> const Expr & { return *ctrl_var_; }

  /**
   * @brief Returns the initial value expression.
   * 
   * @return The initial value expression.
   */
  [[nodiscard]] auto initVal() const -> const Expr & { return *init_val_; }

  /**
   * @brief Returns the condition expression.
   * 
   * @return The condition expression.
   */
  [[nodiscard]] auto cond() const -> const Expr & { return *cond_; }

  /**
   * @brief Returns the body statement.
   * 
   * @return The body statement.
   */
  [[nodiscard]] auto body() const -> const Stmt & { return *body_; }

  /**
   * @brief Returns the direction of the loop.
   * 
   * @return The direction of the loop (true for 'to', false for 'downto').
   */
  [[nodiscard]] auto to() const -> bool { return to_; }

private:
  std::unique_ptr<Expr> ctrl_var_;  ///< The control variable expression.
  std::unique_ptr<Expr> init_val_;  ///< The initial value expression.
  std::unique_ptr<Expr> cond_;      ///< The condition expression.
  std::unique_ptr<Stmt> body_;      ///< The body statement.
  bool to_;                         ///< Specifies the direction of the loop (true for 'to', false for 'downto').
};

/**
 * @brief Represents an assignment statement.
 * 
 * The `AssignStmt` class is a subclass of `Stmt` and represents an assignment statement in the abstract syntax tree (AST).
 * It provides a method `accept` for visitor pattern implementation.
 */
class AssignStmt: public Stmt
{
public:
  void accept(Visitor &v) override;
};

/**
 * \class NormalAssignStmt
 * \brief Represents a normal assignment statement.
 * 
 * This class inherits from the AssignStmt class and provides a representation for a normal assignment statement.
 * It contains a left-hand side (lhs) and a right-hand side (rhs) expression.
 */
class NormalAssignStmt: public AssignStmt
{
public:
  /**
   * \brief Constructs a NormalAssignStmt object.
   * \param lhs The left-hand side expression of the assignment statement.
   * \param rhs The right-hand side expression of the assignment statement.
   */
  NormalAssignStmt(std::unique_ptr<VariableAccess> lhs, std::unique_ptr<Expr> rhs)
    : lhs_(std::move(lhs))
    , rhs_(std::move(rhs))
  {}

  /**
   * \brief Accepts a visitor to perform an operation on the NormalAssignStmt object.
   * \param v The visitor object.
   */
  void accept(Visitor &v) override;

  /**
   * \brief Returns the left-hand side expression of the assignment statement.
   * \return The left-hand side expression.
   */
  [[nodiscard]] auto lhs() const -> const VariableAccess & { return *lhs_; }

  /**
   * \brief Returns the right-hand side expression of the assignment statement.
   * \return The right-hand side expression.
   */
  [[nodiscard]] auto rhs() const -> const Expr & { return *rhs_; }

private:
  std::unique_ptr<VariableAccess> lhs_;  ///< The left-hand side expression.
  std::unique_ptr<Expr> rhs_;            ///< The right-hand side expression.
};

/**
 * @class FuncRetAssignStmt
 * @brief Represents an assignment statement where the value is assigned from a function return.
 * 
 * This class inherits from the AssignStmt class and provides additional functionality for
 * assigning the return value of a function to a variable.
 */
class FuncRetAssignStmt: public AssignStmt
{
public:
  /**
   * @brief Constructs a FuncRetAssignStmt object with the given function identifier and expression.
   * 
   * @param funcid The identifier of the function.
   * @param expr The expression representing the function call.
   */
  FuncRetAssignStmt(std::string funcid, std::unique_ptr<Expr> expr)
    : funcid_(std::move(funcid))
    , expr_(std::move(expr))
  {}

  /**
   * @brief Accepts a visitor object and invokes the appropriate visit method.
   * 
   * This method is part of the visitor design pattern implementation.
   * 
   * @param v The visitor object to accept.
   */
  void accept(Visitor &v) override;

  /**
   * @brief Returns the identifier of the function.
   * 
   * @return The identifier of the function.
   */
  [[nodiscard]] auto funcid() const -> const std::string & { return funcid_; }

  /**
   * @brief Returns the expression representing the function call.
   * 
   * @return The expression representing the function call.
   */
  [[nodiscard]] auto expr() const -> const Expr & { return *expr_; }

private:
  std::string funcid_;          ///< The identifier of the function.
  std::unique_ptr<Expr> expr_;  ///< The expression representing the function call.
};

class ProcCallStmt: public Stmt
{
public:
  ProcCallStmt(std::string procid, std::vector<std::unique_ptr<Expr>> actuals)
    : procid_(std::move(procid))
    , actuals_(std::move(actuals))
  {}

  void accept(Visitor &v) override;

  [[nodiscard]] auto procid() const -> const std::string & { return procid_; }

  [[nodiscard]] auto actuals() const -> const std::vector<std::unique_ptr<Expr>> & { return actuals_; }

private:
  std::string procid_;
  std::vector<std::unique_ptr<Expr>> actuals_;
};

/**
 * @brief Represents a Read statement in the AST.
 * 
 * The ReadStmt class is a derived class of the ProcCallStmt class. It represents a Read statement
 * in the Abstract Syntax Tree (AST). The Read statement is used to read input from the user.
 */
class ReadStmt: public ProcCallStmt
{
public:
  void accept(Visitor &v) override;
};

/**
 * @brief The WriteStmt class represents a statement that calls a procedure to write output.
 * 
 * This class is derived from the ProcCallStmt class.
 * It provides an implementation for the accept() method required by the Visitor pattern.
 */
class WriteStmt: public ProcCallStmt
{
public:
  void accept(Visitor &v) override;
};

/**
 * @brief Represents a statement that reads input from the user.
 * 
 * This class is a subclass of ProcCallStmt and provides an implementation for the accept() method.
 * The accept() method is used to accept a visitor and invoke the appropriate visit method.
 */
class ReadlnStmt: public ProcCallStmt
{
public:
  void accept(Visitor &v) override;
};

/**
 * @brief Represents a writeln statement, which is a type of procedure call statement.
 * 
 * This class inherits from the ProcCallStmt class and provides an implementation for the accept() method.
 */
class WritelnStmt: public ProcCallStmt
{
public:
  void accept(Visitor &v) override;
};

/**
 * @brief Represents a compound statement in the abstract syntax tree.
 * 
 * A compound statement is a block of statements enclosed in curly braces.
 * It can contain zero or more statements.
 */
class CompoundStmt: public Stmt
{
public:
  /**
   * @brief Constructs a CompoundStmt object with the given statements.
   * 
   * @param stmts The statements to be included in the compound statement.
   */
  explicit CompoundStmt(std::vector<std::unique_ptr<Stmt>> stmts)
    : stmts_(std::move(stmts))
  {}

  /**
   * @brief Accepts a visitor and invokes the appropriate visit method.
   * 
   * @param v The visitor object.
   */
  void accept(Visitor &v) override;

  /**
   * @brief Returns the statements contained in the compound statement.
   * 
   * @return A const reference to the vector of statements.
   */
  [[nodiscard]] auto stmts() const -> const std::vector<std::unique_ptr<Stmt>> & { return stmts_; }

private:
  std::vector<std::unique_ptr<Stmt>> stmts_;
};

/**
 * @brief Represents a block of statements.
 * 
 * This class is a subclass of `ASTNode` and represents a block of statements in the abstract syntax tree.
 * It contains a vector of unique pointers to `Stmt` objects, which represent individual statements within the block.
 */
class StmtBlock: public ASTNode
{
public:
  /**
   * @brief Constructs a `StmtBlock` object with the given statements.
   * 
   * @param stmts The vector of unique pointers to `Stmt` objects representing the statements in the block.
   */
  explicit StmtBlock(std::vector<std::unique_ptr<Stmt>> stmts)
    : stmts_(std::move(stmts))
  {}

  /**
   * @brief Accepts a visitor and invokes the appropriate visit method.
   * 
   * This method is used for visitor pattern implementation.
   * It accepts a visitor object and calls the appropriate visit method based on the dynamic type of the object.
   * 
   * @param v The visitor object to accept.
   */
  void accept(Visitor &v) override;

  /**
   * @brief Returns the vector of statements in the block.
   * 
   * @return A const reference to the vector of unique pointers to `Stmt` objects representing the statements in the block.
   */
  [[nodiscard]] auto stmts() const -> const std::vector<std::unique_ptr<Stmt>> & { return stmts_; }

private:
  std::vector<std::unique_ptr<Stmt>> stmts_;
};

/**
 * @brief Represents a block of statements in the abstract syntax tree.
 * 
 * The `Block` class is a derived class of `ASTNode` and represents a block of statements
 * in the abstract syntax tree. It contains a `StmtBlock` object that holds the statements
 * within the block.
 */
class Block: public ASTNode
{
public:
  void accept(Visitor &v) override;

  /**
   * @brief Get the statement block within the block.
   * 
   * @return const StmtBlock& A reference to the statement block within the block.
   */
  [[nodiscard]] auto stmtBlock() const -> const StmtBlock & { return stmt_block_; }

private:
  StmtBlock stmt_block_;
};

/**
 * @brief Represents a program block.
 * 
 * This class inherits from the `Block` class and provides an implementation for the `accept` method.
 * The `accept` method allows a visitor to visit and perform operations on the program block.
 */
class ProgramBlock: public Block
{
public:
  void accept(Visitor &v) override;
};

/**
 * @class ProgramHead
 * @brief Represents the program header in an abstract syntax tree (AST).
 * 
 * The ProgramHead class is a subclass of ASTNode and represents the program header
 * in an abstract syntax tree. It contains the program name and a list of identifiers.
 */
class ProgramHead: public ASTNode
{
public:
  /**
   * @brief Constructs a ProgramHead object with the given program name and identifier list.
   * 
   * @param program_name The name of the program.
   * @param id_list The list of identifiers.
   */
  ProgramHead(std::string program_name, std::vector<std::string> id_list)
    : program_name_(std::move(program_name))
    , id_list_(std::move(id_list))
  {}

  /**
   * @brief Accepts a visitor object and calls the appropriate visit method.
   * 
   * @param v The visitor object.
   */
  void accept(Visitor &v) override;

  /**
   * @brief Returns the program name.
   * 
   * @return The program name.
   */
  [[nodiscard]] auto programName() const -> const std::string & { return program_name_; }

  /**
   * @brief Returns the list of identifiers.
   * 
   * @return The list of identifiers.
   */
  [[nodiscard]] auto idList() const -> const std::vector<std::string> & { return id_list_; }

private:
  std::string program_name_;          ///< The name of the program.
  std::vector<std::string> id_list_;  ///< The list of identifiers.
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
  Program(ProgramHead head, ProgramBlock block)
    : head_(std::move(head))
    , block_(std::move(block))
  {}

  /**
   * @brief Accepts a visitor and calls the appropriate visit method.
   * 
   * @param v The visitor to accept.
   */
  void accept(Visitor &v) override;

  /**
   * @brief Returns the program head.
   * 
   * @return The program head.
   */
  [[nodiscard]] auto head() const -> const ProgramHead & { return head_; }

  /**
   * @brief Returns the program block.
   * 
   * @return The program block.
   */
  [[nodiscard]] auto block() const -> const ProgramBlock & { return block_; }

private:
  ProgramHead head_;    ///< The program head.
  ProgramBlock block_;  ///< The program block.
};

}  // namespace ast

class Visitor
{
public:
  virtual ~Visitor()                             = default;

  virtual void visit(ast::BinaryExpr &node)           = 0;
  virtual void visit(ast::NotExpr &node)              = 0;
  virtual void visit(ast::NegExpr &node)              = 0;
  virtual void visit(ast::UnsignedConstant &node)     = 0;
  virtual void visit(ast::FuncCall &node)             = 0;
  virtual void visit(ast::EntireVariableAccess &node) = 0;

  virtual void visit(ast::IfStmt &node)               = 0;
  virtual void visit(ast::WhileStmt &node)            = 0;
  virtual void visit(ast::ForLoopStmt &node)          = 0;
  virtual void visit(ast::NormalAssignStmt &node)     = 0;
  virtual void visit(ast::FuncRetAssignStmt &node)    = 0;
  virtual void visit(ast::ReadStmt &node)             = 0;
  virtual void visit(ast::WriteStmt &node)            = 0;
  virtual void visit(ast::ReadlnStmt &node)           = 0;
  virtual void visit(ast::WritelnStmt &node)          = 0;
  virtual void visit(ast::CompoundStmt &node)         = 0;
  virtual void visit(ast::StmtBlock &node)            = 0;

  virtual void visit(ast::ProgramHead &node)          = 0;
  virtual void visit(ast::Program &node)              = 0;
};


}  // namespace pascc
