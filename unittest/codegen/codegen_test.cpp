#include "ast/ast.hpp"
#include "codegen/codegen.hpp"
#include "gtest/gtest.h"

auto create_ast() -> std::unique_ptr<pascc::ast::Program>
{
  std::unique_ptr<pascc::ast::Stmt> stmt = std::make_unique<pascc::ast::WritelnStmt>();
  std::vector<std::unique_ptr<pascc::ast::Stmt>> stmts;
  stmts.emplace_back(std::move(stmt));
  std::unique_ptr<pascc::ast::StmtBlock> stmtBlk    = std::make_unique<pascc::ast::StmtBlock>(std::move(stmts));
  std::unique_ptr<pascc::ast::ProgramBlock> progBlk = std::make_unique<pascc::ast::ProgramBlock>(std::move(*stmtBlk));
  std::unique_ptr<pascc::ast::ProgramHead> progHead = std::make_unique<pascc::ast::ProgramHead>("test");
  return std::make_unique<pascc::ast::Program>(std::move(*progHead), std::move(*progBlk));
}

TEST(CodegenTest, example)
{
  pascc::codegen::CodegenVisitor visitor("output.c");
  auto program = create_ast();
  program->accept(visitor);
}