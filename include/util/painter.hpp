#pragma once

#include <fstream>

#include "ast/ast.hpp"

namespace pascc::util {

class PaintVisitor: public Visitor
{
public:
  PaintVisitor() = delete;

  explicit PaintVisitor(const std::string &filename)
    : out_(filename)
  {}

  void visit(ast::Block &node) override;
  void visit(ast::Number &node) override;
  void visit(ast::Constant &node) override;
  void visit(ast::StringLiteral &node) override;


  /// expression
  void visit(ast::Expr &node) override;
  void visit(ast::UnsignedConstant &node) override;
  void visit(ast::BinaryExpr &node) override;
  void visit(ast::UnaryExpr &node) override;
  void visit(ast::FuncCall &node) override;
  void visit(ast::Assignable &node) override;
  void visit(ast::AssignableId &node) override;
  void visit(ast::IndexedVar &node) override;
  void visit(ast::FieldDesignator &node) override;


  /// const declaration
  void visit(ast::ConstDecl &node) override;
  void visit(ast::ConstDeclPart &node) override;


  /// type declaration
  void visit(ast::TypeId &node) override;
  void visit(ast::Period &node) override;
  void visit(ast::ArrayType &node) override;
  void visit(ast::RecordType &node) override;
  void visit(ast::TypeDecl &node) override;
  void visit(ast::TypeDeclPart &node) override;
  void visit(ast::TypeDenoter &node) override;


  /// var declaration
  void visit(ast::VarDeclPart &node) override;
  void visit(ast::ValueParamSpec &node) override;
  void visit(ast::VarParamSpec &node) override;
  void visit(ast::VarDecl &node) override;


  /// subprogram declaration
  void visit(ast::ProcHead &node) override;
  void visit(ast::ProcBlock &node) override;
  void visit(ast::ProcDecl &node) override;
  void visit(ast::FuncHead &node) override;
  void visit(ast::FuncBlock &node) override;
  void visit(ast::FuncDecl &node) override;
  void visit(ast::FormalParam &node) override;
  void visit(ast::SubprogDecl &node) override;
  void visit(ast::SubprogDeclPart &node) override;


  /// statement
  void visit(ast::Stmt &node) override;
  // conditional statement
  void visit(ast::IfStmt &node) override;
  void visit(ast::CaseStmt &node) override;
  void visit(ast::CaseListElement &node) override;
  // repetitive statement
  void visit(ast::RepeatStmt &node) override;
  void visit(ast::WhileStmt &node) override;
  void visit(ast::ForStmt &node) override;
  // simple statement
  void visit(ast::AssignStmt &node) override;
  void visit(ast::ProcCallStmt &node) override;
  void visit(ast::ReadStmt &node) override;
  void visit(ast::WriteStmt &node) override;
  void visit(ast::ReadlnStmt &node) override;
  void visit(ast::WritelnStmt &node) override;
  void visit(ast::ExitStmt &node) override;
  void visit(ast::CompoundStmt &node) override;
  void visit(ast::StmtPart &node) override;

  /// program
  void visit(ast::ProgramBlock &node) override;
  void visit(ast::ProgramHead &node) override;
  void visit(ast::Program &node) override;

private:
  std::ofstream out_;
};

}  // namespace pascc::util