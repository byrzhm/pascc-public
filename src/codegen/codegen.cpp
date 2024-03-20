#include <iostream>

#include "codegen/codegen.hpp"

namespace {

constexpr int INDENT_SIZE = 2;

class IndentGuard
{
public:
  IndentGuard(int *indent, int indent_size)
    : indent_(indent)
    , indent_size_(indent_size)
  {
    *indent += indent_size_;
  }

  ~IndentGuard()
  {
    *indent_ -= indent_size_;
  }

private:
  int *indent_ = nullptr;
  int indent_size_;
};

}  // namespace

namespace pascc::codegen {

static auto indent(int n) -> std::string
{
  return std::string(n, ' ');
}

auto CodegenVisitor::println(const std::string &str) -> void
{
  print(str + "\n");
}

auto CodegenVisitor::print(const std::string &str) -> void
{
  (file_output_ ? fout_ : std::cout) << indent(indent_) << str;
}

// ! Expr 再等等

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

void CodegenVisitor::visit([[maybe_unused]] ast::Assignable &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::AssignableId &node)
{
  throw std::runtime_error("Not implemented");
}

// ! 控制语句再等等

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

// TODO(fpy&dly): implement this
void CodegenVisitor::visit([[maybe_unused]] ast::ProcCallStmt &node)
{
  throw std::runtime_error("Not implemented");
}

// TODO(fpy&dly): implement this
void CodegenVisitor::visit([[maybe_unused]] ast::ReadStmt &node)
{
  throw std::runtime_error("Not implemented");
}

// TODO(fpy&dly): implement this
void CodegenVisitor::visit([[maybe_unused]] ast::WriteStmt &node)
{
  throw std::runtime_error("Not implemented");
}

// TODO(fpy&dly): implement this
void CodegenVisitor::visit([[maybe_unused]] ast::ReadlnStmt &node)
{
  throw std::runtime_error("Not implemented");
}

// TODO(fpy&dly): writeln arguments and remove [[maybe_unused]]
void CodegenVisitor::visit([[maybe_unused]] ast::WritelnStmt &node)
{
  // print("printf(");
  // for (...)
  // 1st actual
  // 2nd actual
  // ...
  // print(")\n");
  println(R"(printf("\n");)");
}

// TODO(fpy&dly): implement this
void CodegenVisitor::visit([[maybe_unused]] ast::CompoundStmt &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::Block &node)
{
  throw std::runtime_error("Not implemented");
}

// DON'T MODIFY
void CodegenVisitor::visit(ast::StmtPart &node)
{
  IndentGuard ig(&indent_, INDENT_SIZE);
  for (const auto &stmt : node.stmts())
  {
    stmt->accept(*this);
  }
}

// TODO(fpy&dly): const-def-part type-def-part var-decl-part subprog-decl-part
void CodegenVisitor::visit(ast::ProgramBlock &node)
{
  println("int main() {");
  node.stmtPart().accept(*this);
  println("}");
}

// ? param list
void CodegenVisitor::visit(ast::ProgramHead &node)
{
  println("/**");
  println(" * Program name: " + node.programName());
  println(" */");
  println("#include <stdio.h>");
}

void CodegenVisitor::visit(ast::Program &node)
{
  node.head().accept(*this);
  node.block().accept(*this);
}


}  // namespace pascc::codegen