#pragma once

#include "ast/ast.hpp"
#include "semant/semant_context.hpp"

namespace pascc::semant {

class SemantVisitor: public Visitor
{
public:
  SemantVisitor() = default;

  void visit(ast::Expr &node) override;
  void visit(ast::BinaryExpr &node) override;
  void visit(ast::UnaryExpr &node) override;
  void visit(ast::UnsignedConstant &node) override;
  void visit(ast::FuncCall &node) override;
  void visit(ast::VariableAccess &node) override;
  void visit(ast::EntireVariableAccess &node) override;

  void visit(ast::Stmt &node) override;
  void visit(ast::IfStmt &node) override;
  void visit(ast::WhileStmt &node) override;
  void visit(ast::ForStmt &node) override;
  void visit(ast::NormalAssignStmt &node) override;
  void visit(ast::FuncRetAssignStmt &node) override;
  void visit(ast::ProcCallStmt &node) override;
  void visit(ast::ReadStmt &node) override;
  void visit(ast::WriteStmt &node) override;
  void visit(ast::ReadlnStmt &node) override;
  void visit(ast::WritelnStmt &node) override;
  void visit(ast::CompoundStmt &node) override;

  void visit(ast::Block &node) override;
  void visit(ast::StmtBlock &node) override;

  void visit(ast::ProgramBlock &node) override;
  void visit(ast::ProgramHead &node) override;
  void visit(ast::Program &node) override;

  auto isOk() -> bool;

private:
  SemantContext context_;
};

}  // namespace pascc::semant
