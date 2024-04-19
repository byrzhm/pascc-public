#include "codegen/codegen_context.hpp"

namespace pascc::codegen {

CodegenContext::CodegenContext()
{
  typetab_.insert("integer", &util::SymType::IntegerType());
  typetab_.insert("real", &util::SymType::RealType());
  typetab_.insert("boolean", &util::SymType::BooleanType());
  typetab_.insert("char", &util::SymType::CharType());
  typetab_.insert("string", &util::SymType::StringType());
}

void CodegenContext::enterScope()
{
  typetab_.enterScope();
  consttab_.enterScope();
  vartab_.enterScope();
  subprogtab_.enterScope();
}

void CodegenContext::exitScope()
{
  typetab_.exitScope();
  consttab_.exitScope();
  vartab_.exitScope();
  subprogtab_.exitScope();
}

}  // namespace pascc::codegen