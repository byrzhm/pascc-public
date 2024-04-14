#pragma once

#include <fstream>

#include "ast/ast.hpp"

auto strip(const std::string &str) -> std::string
{
  std::string trimmed = str;
  trimmed.erase(0, trimmed.find_first_not_of(" \n\r\t"));
  trimmed.erase(trimmed.find_last_not_of(" \n\r\t") + 1);
  return trimmed;
}

auto read_expected_code(const std::string &filepath) -> std::string
{
  std::ifstream file(filepath);
  std::string expected_code(
      (std::istreambuf_iterator<char>(file)),
      std::istreambuf_iterator<char>()
  );
  return expected_code;
}

auto create_example_ast() -> std::unique_ptr<pascc::ast::Program>
{
  // writeln();
  std::unique_ptr<pascc::ast::Stmt> stmt = std::make_unique<pascc::ast::WritelnStmt>();

  // add writeln() to stmts
  std::vector<std::unique_ptr<pascc::ast::Stmt>> stmts;
  stmts.emplace_back(std::move(stmt));

  // compound statement
  std::unique_ptr<pascc::ast::CompoundStmt> compoundStmt =
      std::make_unique<pascc::ast::CompoundStmt>(std::move(stmts));

  // stmt part
  std::unique_ptr<pascc::ast::StmtPart> stmtPart =
      std::make_unique<pascc::ast::StmtPart>(std::move(compoundStmt));

  // block
  std::unique_ptr<pascc::ast::Block> blk =
      std::make_unique<pascc::ast::Block>(nullptr, nullptr, nullptr, nullptr, std::move(stmtPart));

  // program block
  std::unique_ptr<pascc::ast::ProgramBlock> progBlk =
      std::make_unique<pascc::ast::ProgramBlock>(std::move(blk));

  // program head
  std::unique_ptr<pascc::ast::ProgramHead> progHead =
      std::make_unique<pascc::ast::ProgramHead>("test");

  // program
  return std::make_unique<pascc::ast::Program>(std::move(progHead), std::move(progBlk));
}

// construct program with writeln("Hello World");
auto create_ast_helloworld() -> std::unique_ptr<pascc::ast::Program>
{
  // writeln("Hello World");
  std::unique_ptr<pascc::ast::Expr> actual =
      std::make_unique<pascc::ast::StringLiteral>("Hello World");
  std::vector<std::unique_ptr<pascc::ast::Expr>> actuals;
  actuals.emplace_back(std::move(actual));
  std::unique_ptr<pascc::ast::Stmt> stmt = std::make_unique<pascc::ast::WritelnStmt>(std::move(actuals));

  // add writeln("Hello World") to stmts
  std::vector<std::unique_ptr<pascc::ast::Stmt>> stmts;
  stmts.emplace_back(std::move(stmt));

  // compound statement
  std::unique_ptr<pascc::ast::CompoundStmt> compoundStmt =
      std::make_unique<pascc::ast::CompoundStmt>(std::move(stmts));

  // stmt part
  std::unique_ptr<pascc::ast::StmtPart> stmtPart =
      std::make_unique<pascc::ast::StmtPart>(std::move(compoundStmt));

  // block
  std::unique_ptr<pascc::ast::Block> blk =
      std::make_unique<pascc::ast::Block>(nullptr, nullptr, nullptr, nullptr, std::move(stmtPart));

  // program block
  std::unique_ptr<pascc::ast::ProgramBlock> progBlk =
      std::make_unique<pascc::ast::ProgramBlock>(std::move(blk));

  // program head
  std::unique_ptr<pascc::ast::ProgramHead> progHead =
      std::make_unique<pascc::ast::ProgramHead>("test");

  // program
  return std::make_unique<pascc::ast::Program>(std::move(progHead), std::move(progBlk));
}