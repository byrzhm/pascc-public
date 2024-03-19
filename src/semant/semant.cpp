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

void SemantVisitor::visit([[maybe_unused]] ast::VariableAccess &node)
{
}

void SemantVisitor::visit([[maybe_unused]] ast::EntireVariableAccess &node)
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

void SemantVisitor::visit([[maybe_unused]] ast::NormalAssignStmt &node)
{
}

void SemantVisitor::visit([[maybe_unused]] ast::FuncRetAssignStmt &node)
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

void SemantVisitor::visit([[maybe_unused]] ast::StmtBlock &node)
{
}

void SemantVisitor::visit([[maybe_unused]] ast::ProgramBlock &node)
{
  SemantVisitor::visit(node.stmtBlock());
}

void SemantVisitor::visit([[maybe_unused]] ast::ProgramHead &node)
{
}

void SemantVisitor::visit([[maybe_unused]] ast::Program &node)
{
  //Program -> ProgramHead ';' ProgramBlock '.'
  SemantVisitor::visit(node.head());
  SemantVisitor::visit(node.block());
}

}  // namespace pascc::semant
