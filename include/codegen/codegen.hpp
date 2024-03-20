#pragma once

#include <fstream>

#include "ast/ast.hpp"

namespace pascc::codegen {

class CodegenVisitor: public Visitor
{
public:
  explicit CodegenVisitor(const std::string &filepath)
  {
    if (!filepath.empty()) {
      file_output_ = true;
      fout_.open(filepath);
    }
  }

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

private:
  auto println(const std::string &str) -> void;
  auto print(const std::string &str) -> void;

  bool file_output_ = false;
  std::ofstream fout_;
  int indent_ = 0;
};

}  // namespace pascc::codegen
