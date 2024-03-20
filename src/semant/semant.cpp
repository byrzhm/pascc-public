#include "semant/semant.hpp"

namespace pascc::semant {

auto SemantVisitor::isOk() -> bool
{
  return context_.error_count() == 0;
}

void SemantVisitor::visit([[maybe_unused]] ast::Expr &node)
{
}

void SemantVisitor::visit([[maybe_unused]] ast::BinaryExpr &node)
{
}

void SemantVisitor::visit([[maybe_unused]] ast::UnaryExpr &node)
{
}

void SemantVisitor::visit([[maybe_unused]] ast::UnsignedConstant &node)
{
}

void SemantVisitor::visit([[maybe_unused]] ast::FuncCall &node)
{
}

void SemantVisitor::visit([[maybe_unused]] ast::Assignable &node)
{
}

void SemantVisitor::visit([[maybe_unused]] ast::AssignableId &node)
{
}

void SemantVisitor::visit([[maybe_unused]] ast::Stmt &node)
{
}

void SemantVisitor::visit([[maybe_unused]] ast::IfStmt &node)
{
}

void SemantVisitor::visit([[maybe_unused]] ast::WhileStmt &node)
{
}

void SemantVisitor::visit([[maybe_unused]] ast::ForStmt &node)
{
}

void SemantVisitor::visit([[maybe_unused]] ast::ProcCallStmt &node)
{
}

void SemantVisitor::visit([[maybe_unused]] ast::ReadStmt &node)
{
}

void SemantVisitor::visit([[maybe_unused]] ast::WriteStmt &node)
{
}

void SemantVisitor::visit([[maybe_unused]] ast::ReadlnStmt &node)
{
}

void SemantVisitor::visit([[maybe_unused]] ast::WritelnStmt &node)
{
}

void SemantVisitor::visit([[maybe_unused]] ast::CompoundStmt &node)
{
}

void SemantVisitor::visit([[maybe_unused]] ast::Block &node)
{
}

void SemantVisitor::visit([[maybe_unused]] ast::StmtPart &node)
{
}

void SemantVisitor::visit([[maybe_unused]] ast::ProgramBlock &node)
{
}

void SemantVisitor::visit([[maybe_unused]] ast::ProgramHead &node)
{
}

void SemantVisitor::visit([[maybe_unused]] ast::Program &node)
{
  //Program -> ProgramHead ';' ProgramBlock '.'
  node.head().accept(*this);
  node.block().accept(*this);
}

}  // namespace pascc::semant
