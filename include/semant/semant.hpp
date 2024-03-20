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
  void visit(ast::Assignable &node) override;
  void visit(ast::AssignableId &node) override;

  void visit(ast::Stmt &node) override;
  void visit(ast::IfStmt &node) override;
  void visit(ast::WhileStmt &node) override;
  void visit(ast::ForStmt &node) override;
  void visit(ast::ProcCallStmt &node) override;
  void visit(ast::ReadStmt &node) override;
  void visit(ast::WriteStmt &node) override;
  void visit(ast::ReadlnStmt &node) override;
  void visit(ast::WritelnStmt &node) override;
  void visit(ast::CompoundStmt &node) override;

  void visit(ast::Block &node) override;
  void visit(ast::StmtPart &node) override;

  void visit(ast::ProgramBlock &node) override;
  void visit(ast::ProgramHead &node) override;
  void visit(ast::Program &node) override;

  auto isOk() -> bool;

private:
  SemantContext context_;
};

}  // namespace pascc::semant
