#include "semant/semant.hpp"

namespace pascc::semant {

auto SemantVisitor::isOk() -> bool
{
  return context_.error_count() == 0;
}

void SemantVisitor::visit([[maybe_unused]] ast::Block &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::Number &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::Constant &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::StringLiteral &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::Expr &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::UnsignedConstant &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::BinaryExpr &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::UnaryExpr &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::FuncCall &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::Assignable &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::AssignableId &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::IndexedVar &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::FieldDesignator &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ConstDecl &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ConstDeclPart &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::TypeId &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::Period &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ArrayType &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::RecordType &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::TypeDecl &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::TypeDeclPart &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::TypeDenoter &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::VarDeclPart &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ValueParamSpec &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::VarParamSpec &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::VarDecl &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ProcHead &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ProcBlock &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ProcDecl &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::FuncHead &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::FuncBlock &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::FuncDecl &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::FormalParam &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::SubprogDecl &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::SubprogDeclPart &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::Stmt &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::IfStmt &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::CaseStmt &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::CaseListElement &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::RepeatStmt &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::WhileStmt &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ForStmt &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::AssignStmt &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ProcCallStmt &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ReadStmt &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::WriteStmt &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ReadlnStmt &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::WritelnStmt &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ExitStmt &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::CompoundStmt &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::StmtPart &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ProgramBlock &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ProgramHead &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::Program &node)
{
  //Program -> ProgramHead ';' ProgramBlock '.'
  node.head().accept(*this);
  node.block().accept(*this);
}

}  // namespace pascc::semant
