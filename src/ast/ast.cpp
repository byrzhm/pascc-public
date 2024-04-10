#include "ast/ast.hpp"

namespace pascc::ast {

auto operator<<(std::ostream &os, BinOp op) -> std::ostream &
{
  switch (op) {
    case BinOp::PLUS: os << "+"; break;
    case BinOp::MINUS: os << "-"; break;
    case BinOp::MUL: os << "*"; break;
    case BinOp::FDIV: os << "/"; break;
    case BinOp::IDIV: os << "div"; break;
    case BinOp::MOD: os << "mod"; break;
    case BinOp::AND: os << "and"; break;
    case BinOp::OR: os << "or"; break;
    case BinOp::EQ: os << "="; break;
    case BinOp::NE: os << "<>"; break;
    case BinOp::LT: os << "<"; break;
    case BinOp::GT: os << ">"; break;
    case BinOp::LE: os << "<="; break;
    case BinOp::GE: os << ">="; break;
  }
  return os;
}

auto operator<<(std::ostream &os, UnaryOp op) -> std::ostream &
{
  switch (op) {
    case UnaryOp::NOT: os << "not"; break;
    case UnaryOp::MINUS: os << "-"; break;
    case UnaryOp::PLUS: os << "+"; break;
  }
  return os;
}

void Block::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void BoolExpr::accept(Visitor &visitor)
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

void Number::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void StringLiteral::accept(Visitor &visitor)
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

void AssignableId::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void IndexedVar::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void FieldDesignator::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void Constant::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void ConstDecl::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void ConstDeclPart::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void TypeId::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void Period::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void ArrayType::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void RecordType::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void TypeDecl::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void TypeDeclPart::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void VarDecl::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void VarDeclPart::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void ValueParamSpec::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void VarParamSpec::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void ProcHead::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void ProcBlock::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void ProcDecl::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void FuncHead::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void FuncBlock::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void FuncDecl::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void SubprogDeclPart::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void AssignStmt::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void ProcCallStmt::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void ReadStmt::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void ReadlnStmt::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void WriteStmt::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void WritelnStmt::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void ExitStmt::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void IfStmt::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void CaseListElement::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void CaseStmt::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void RepeatStmt::accept(Visitor &visitor)
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

void CompoundStmt::accept(Visitor &visitor)
{
  visitor.visit(*this);
}

void StmtPart::accept(Visitor &visitor)
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