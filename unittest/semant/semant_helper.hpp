#include "ast/ast.hpp"

auto create_example_ast() -> std::unique_ptr<pascc::ast::Program>
{
  std::unique_ptr<pascc::ast::Stmt> stmt = std::make_unique<pascc::ast::WritelnStmt>();
  std::vector<std::unique_ptr<pascc::ast::Stmt>> stmts;
  stmts.emplace_back(std::move(stmt));
  std::unique_ptr<pascc::ast::CompoundStmt> compoundStmt = std::make_unique<pascc::ast::CompoundStmt>(std::move(stmts));
  std::unique_ptr<pascc::ast::StmtPart> stmtPart         = std::make_unique<pascc::ast::StmtPart>(std::move(compoundStmt));
  std::unique_ptr<pascc::ast::Block> blk                 = std::make_unique<pascc::ast::Block>(nullptr, nullptr, nullptr, nullptr, std::move(stmtPart));
  std::unique_ptr<pascc::ast::ProgramBlock> progBlk      = std::make_unique<pascc::ast::ProgramBlock>(std::move(blk));
  std::unique_ptr<pascc::ast::ProgramHead> progHead = std::make_unique<pascc::ast::ProgramHead>("test");
  return std::make_unique<pascc::ast::Program>(std::move(progHead), std::move(progBlk));
}
