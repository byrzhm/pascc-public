#pragma once

#include "ast/ast.hpp"

namespace pascc {

/**
 * @brief Visitor 抽象基类
 * @anchor Visitor
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
  virtual void visit(ast::BoolExpr &node)         = 0;
  virtual void visit(ast::UnsignedConstant &node) = 0;
  virtual void visit(ast::BinaryExpr &node)       = 0;
  virtual void visit(ast::UnaryExpr &node)        = 0;
  virtual void visit(ast::FuncCall &node)         = 0;
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
  virtual void visit(ast::SubprogDeclPart &node) = 0;


  /// statement
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

// shared visitor methods
#define SHARED_VISITOR_METHODS                      \
  void visit(ast::Block &node) override;            \
  void visit(ast::Number &node) override;           \
  void visit(ast::Constant &node) override;         \
  void visit(ast::StringLiteral &node) override;    \
                                                    \
                                                    \
  void visit(ast::BoolExpr &node) override;         \
  void visit(ast::UnsignedConstant &node) override; \
  void visit(ast::BinaryExpr &node) override;       \
  void visit(ast::UnaryExpr &node) override;        \
  void visit(ast::FuncCall &node) override;         \
  void visit(ast::AssignableId &node) override;     \
  void visit(ast::IndexedVar &node) override;       \
  void visit(ast::FieldDesignator &node) override;  \
                                                    \
                                                    \
  void visit(ast::ConstDecl &node) override;        \
  void visit(ast::ConstDeclPart &node) override;    \
                                                    \
                                                    \
  void visit(ast::TypeId &node) override;           \
  void visit(ast::Period &node) override;           \
  void visit(ast::ArrayType &node) override;        \
  void visit(ast::RecordType &node) override;       \
  void visit(ast::TypeDecl &node) override;         \
  void visit(ast::TypeDeclPart &node) override;     \
                                                    \
                                                    \
  void visit(ast::VarDeclPart &node) override;      \
  void visit(ast::ValueParamSpec &node) override;   \
  void visit(ast::VarParamSpec &node) override;     \
  void visit(ast::VarDecl &node) override;          \
                                                    \
                                                    \
  void visit(ast::ProcHead &node) override;         \
  void visit(ast::ProcBlock &node) override;        \
  void visit(ast::ProcDecl &node) override;         \
  void visit(ast::FuncHead &node) override;         \
  void visit(ast::FuncBlock &node) override;        \
  void visit(ast::FuncDecl &node) override;         \
  void visit(ast::SubprogDeclPart &node) override;  \
                                                    \
                                                    \
  void visit(ast::IfStmt &node) override;           \
  void visit(ast::CaseStmt &node) override;         \
  void visit(ast::CaseListElement &node) override;  \
                                                    \
  void visit(ast::RepeatStmt &node) override;       \
  void visit(ast::WhileStmt &node) override;        \
  void visit(ast::ForStmt &node) override;          \
                                                    \
  void visit(ast::AssignStmt &node) override;       \
  void visit(ast::ProcCallStmt &node) override;     \
  void visit(ast::ReadStmt &node) override;         \
  void visit(ast::WriteStmt &node) override;        \
  void visit(ast::ReadlnStmt &node) override;       \
  void visit(ast::WritelnStmt &node) override;      \
  void visit(ast::ExitStmt &node) override;         \
  void visit(ast::CompoundStmt &node) override;     \
  void visit(ast::StmtPart &node) override;         \
                                                    \
                                                    \
  void visit(ast::ProgramBlock &node) override;     \
  void visit(ast::ProgramHead &node) override;      \
  void visit(ast::Program &node) override;
