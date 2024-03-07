#include "codegen/codegen.hpp"

namespace pascc::codegen {

static auto indent(int n) -> std::string
{
  return std::string(n, ' ');
}

auto CodegenVisitor::println(const std::string &str) -> void
{
  fout_ << indent(indent_) << str << "\n";
}

void CodegenVisitor::visit([[maybe_unused]] ast::Expr &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::BinaryExpr &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::UnaryExpr &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::UnsignedConstant &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::FuncCall &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::VariableAccess &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::EntireVariableAccess &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::Stmt &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::IfStmt &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::WhileStmt &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::ForStmt &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::NormalAssignStmt &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::FuncRetAssignStmt &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::ProcCallStmt &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::ReadStmt &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::WriteStmt &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::ReadlnStmt &node)
{
  throw std::runtime_error("Not implemented");
}

// TODO(who): writeln arguments and remove [[maybe_unused]]
void CodegenVisitor::visit([[maybe_unused]] ast::WritelnStmt &node)
{
  println(R"(printf("\n");)");
}

void CodegenVisitor::visit([[maybe_unused]] ast::CompoundStmt &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::Block &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit(ast::StmtBlock &node)
{
  indent_ += 2;
  for (const auto &stmt : node.stmts()) {
    stmt->accept(*this);
  }
  indent_ -= 2;
}

void CodegenVisitor::visit(ast::ProgramBlock &node)
{
  println("int main() {");
  node.stmtBlock().accept(*this);
  println("}");
}

void CodegenVisitor::visit(ast::ProgramHead &node)
{
  println("/**");
  println(" * Program Head" + node.programName());
  println(" */");
  println("#include <stdio.h>");
}

void CodegenVisitor::visit(ast::Program &node)
{
  node.head().accept(*this);
  node.block().accept(*this);
}


}  // namespace pascc::codegen