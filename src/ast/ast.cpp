#include "ast/ast.hpp"

namespace pascc::ast {

void Expr::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void BinaryExpr::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void UnaryExpr::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void UnsignedConstant::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void FuncCall::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void VariableAccess::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void EntireVariableAccess::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void Stmt::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void IfStmt::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void WhileStmt::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void ForStmt::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void ProcCallStmt::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void WritelnStmt::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void Block::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void StmtBlock::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void ProgramBlock::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void ProgramHead::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void Program::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

}  // namespace pascc::ast