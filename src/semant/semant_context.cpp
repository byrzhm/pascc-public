#include "semant/semant_context.hpp"
#include <sstream>

namespace pascc::semant {

SemantContext::SemantContext()
{
  typetab_.insert("integer", &util::SymType::IntegerType());
  typetab_.insert("real", &util::SymType::RealType());
  typetab_.insert("boolean", &util::SymType::BooleanType());
  typetab_.insert("char", &util::SymType::CharType());
  typetab_.insert("string", &util::SymType::StringType());
}

void SemantContext::enterScope()
{
  typetab_.enterScope();
  consttab_.enterScope();
  vartab_.enterScope();
  subprogtab_.enterScope();
}

void SemantContext::exitScope()
{
  typetab_.exitScope();
  consttab_.exitScope();
  vartab_.exitScope();
  subprogtab_.exitScope();
}

auto SemantContext::topFunc() -> std::string
{
  if (functions_.empty()) {
    return "";
  }
  return functions_.top();
}

void SemantContext::pushFunc(const std::string &func_name)
{
  functions_.emplace(func_name);
}

void SemantContext::popFunc()
{
  functions_.pop();
}

void SemantContext::genErrorMsg(const parse::location &loc, const std::string &error_msg)
{
  std::stringstream sstr;
  sstr << loc << ": " << error_msg;
  error_msgs_.emplace_back(sstr.str());
}

}  // namespace pascc::semant