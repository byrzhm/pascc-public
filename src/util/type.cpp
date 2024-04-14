#include "util/type.hpp"

namespace pascc::util {

void TypeVisitor::visit([[maybe_unused]] ast::TypeId &node) {
  // TODO(): Implement this
}

void TypeVisitor::visit([[maybe_unused]] ast::Period &node) {
  // TODO(): Implement this
}

void TypeVisitor::visit([[maybe_unused]] ast::ArrayType &node) {
  // TODO(): Implement this
}

void TypeVisitor::visit([[maybe_unused]] ast::RecordType &node) {
  // TODO(): Implement this
}

void TypeVisitor::visit([[maybe_unused]] ast::VarDecl &node) {
  // TODO(): Implement this
}


auto TypeComparator::operator()(
    const SymType &lhs,
    const SymType &rhs
) -> bool
{
  // TODO(): Implement this
  (void)lhs;
  (void)rhs;
  return false;
}

auto TypeComparator::cast(
    const SymType &from,
    const SymType &to
) -> bool
{
  // TODO(): Implement this
  (void)from;
  (void)to;
  return false;
}


//************************************************************************
//************************* Dummy implementations ************************
//************************************************************************
void visit([[maybe_unused]] ast::Block &node) {}
void visit([[maybe_unused]] ast::Number &node) {}
void visit([[maybe_unused]] ast::Constant &node) {}
void visit([[maybe_unused]] ast::StringLiteral &node) {}
void visit([[maybe_unused]] ast::BoolExpr &node) {}
void visit([[maybe_unused]] ast::UnsignedConstant &node) {}
void visit([[maybe_unused]] ast::BinaryExpr &node) {}
void visit([[maybe_unused]] ast::UnaryExpr &node) {}
void visit([[maybe_unused]] ast::FuncCall &node) {}
void visit([[maybe_unused]] ast::AssignableId &node) {}
void visit([[maybe_unused]] ast::IndexedVar &node) {}
void visit([[maybe_unused]] ast::FieldDesignator &node) {}
void visit([[maybe_unused]] ast::ConstDecl &node) {}
void visit([[maybe_unused]] ast::ConstDeclPart &node) {}
// void visit([[maybe_unused]] ast::TypeId &node) {}
// void visit([[maybe_unused]] ast::Period &node) {}
// void visit([[maybe_unused]] ast::ArrayType &node) {}
// void visit([[maybe_unused]] ast::RecordType &node) {}
void visit([[maybe_unused]] ast::TypeDecl &node) {}
void visit([[maybe_unused]] ast::TypeDeclPart &node) {}
void visit([[maybe_unused]] ast::VarDeclPart &node) {}
void visit([[maybe_unused]] ast::ValueParamSpec &node) {}
void visit([[maybe_unused]] ast::VarParamSpec &node) {}
// void visit([[maybe_unused]] ast::VarDecl &node) {}
void visit([[maybe_unused]] ast::ProcHead &node) {}
void visit([[maybe_unused]] ast::ProcBlock &node) {}
void visit([[maybe_unused]] ast::ProcDecl &node) {}
void visit([[maybe_unused]] ast::FuncHead &node) {}
void visit([[maybe_unused]] ast::FuncBlock &node) {}
void visit([[maybe_unused]] ast::FuncDecl &node) {}
void visit([[maybe_unused]] ast::SubprogDeclPart &node) {}
void visit([[maybe_unused]] ast::IfStmt &node) {}
void visit([[maybe_unused]] ast::CaseStmt &node) {}
void visit([[maybe_unused]] ast::CaseListElement &node) {}
void visit([[maybe_unused]] ast::RepeatStmt &node) {}
void visit([[maybe_unused]] ast::WhileStmt &node) {}
void visit([[maybe_unused]] ast::ForStmt &node) {}
void visit([[maybe_unused]] ast::AssignStmt &node) {}
void visit([[maybe_unused]] ast::ProcCallStmt &node) {}
void visit([[maybe_unused]] ast::ReadStmt &node) {}
void visit([[maybe_unused]] ast::WriteStmt &node) {}
void visit([[maybe_unused]] ast::ReadlnStmt &node) {}
void visit([[maybe_unused]] ast::WritelnStmt &node) {}
void visit([[maybe_unused]] ast::ExitStmt &node) {}
void visit([[maybe_unused]] ast::CompoundStmt &node) {}
void visit([[maybe_unused]] ast::StmtPart &node) {}
void visit([[maybe_unused]] ast::ProgramBlock &node) {}
void visit([[maybe_unused]] ast::ProgramHead &node) {}
void visit([[maybe_unused]] ast::Program &node) {}

}  // namespace pascc::util