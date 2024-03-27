#pragma once

#include "ast/ast.hpp"

// Read expected code from file
#include <fstream>

auto read_expected(const std::string &filepath) -> std::string
{
  std::string expected_code;
  std::ifstream file(filepath);
  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      expected_code += line += '\n';
    }
    file.close();
  } else {
    std::cout << "Failed to open file" << '\n';
  }
  return expected_code;
}

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

auto get_example_expected_code() -> std::string
{
  std::string code = "\
/**\n\
 * Program name: test\n\
 */\n\
#include <stdio.h>\n\
int main() {\n\
  printf(\"\\n\");\n\
}\n";
  return code;
}

// construct program with writeln("Hello World");
auto create_ast_helloworld() -> std::unique_ptr<pascc::ast::Program>
{
  std::unique_ptr<pascc::ast::Expr> actual = std::make_unique<pascc::ast::StringLiteral>("Hello World");
  std::vector<std::unique_ptr<pascc::ast::Expr>> actuals;
  actuals.emplace_back(std::move(actual));
  std::unique_ptr<pascc::ast::Stmt> stmt = std::make_unique<pascc::ast::WritelnStmt>(std::move(actuals));
  std::vector<std::unique_ptr<pascc::ast::Stmt>> stmts;
  stmts.emplace_back(std::move(stmt));
  std::unique_ptr<pascc::ast::CompoundStmt> compoundStmt = std::make_unique<pascc::ast::CompoundStmt>(std::move(stmts));
  std::unique_ptr<pascc::ast::StmtPart> stmtPart         = std::make_unique<pascc::ast::StmtPart>(std::move(compoundStmt));
  std::unique_ptr<pascc::ast::Block> blk                 = std::make_unique<pascc::ast::Block>(nullptr, nullptr, nullptr, nullptr, std::move(stmtPart));
  std::unique_ptr<pascc::ast::ProgramBlock> progBlk      = std::make_unique<pascc::ast::ProgramBlock>(std::move(blk));
  std::unique_ptr<pascc::ast::ProgramHead> progHead      = std::make_unique<pascc::ast::ProgramHead>("test");
  return std::make_unique<pascc::ast::Program>(std::move(progHead), std::move(progBlk));
}