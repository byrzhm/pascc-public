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

void CodegenVisitor::visit([[maybe_unused]] ast::Block &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::Number &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::Constant &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::StringLiteral &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::Expr &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::UnsignedConstant &node)
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

void CodegenVisitor::visit([[maybe_unused]] ast::IndexedVar &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::FieldDesignator &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::ConstDecl &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::ConstDeclPart &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::TypeId &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::Period &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::ArrayType &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::RecordType &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::TypeDecl &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::TypeDeclPart &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::TypeDenoter &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::VarDeclPart &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::ValueParamSpec &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::VarParamSpec &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::VarDecl &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::ProcHead &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::ProcBlock &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::ProcDecl &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::FuncHead &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::FuncBlock &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::FuncDecl &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::FormalParam &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::SubprogDecl &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::SubprogDeclPart &node)
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

void CodegenVisitor::visit([[maybe_unused]] ast::CaseStmt &node)
{
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::CaseListElement &node)
{
  throw std::runtime_error("Not implemented");
}


void CodegenVisitor::visit([[maybe_unused]] ast::RepeatStmt &node)
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

void CodegenVisitor::visit([[maybe_unused]] ast::AssignStmt &node)
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

void CodegenVisitor::visit([[maybe_unused]] ast::ExitStmt &node)
{
  throw std::runtime_error("Not implemented");
}

// TODO(fpy&dly): implement this
void CodegenVisitor::visit([[maybe_unused]] ast::CompoundStmt &node)
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