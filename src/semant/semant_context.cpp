#include "semant/semant_context.hpp"
#include <sstream>

namespace pascc::semant {

SemantContext::SemantContext()
{
  typetab_.insert("integer", util::SymType{util::BuiltInType{util::BasicType::INTEGER}});
  typetab_.insert("real", util::SymType{util::BuiltInType{util::BasicType::REAL}});
  typetab_.insert("boolean", util::SymType{util::BuiltInType{util::BasicType::BOOLEAN}});
  typetab_.insert("char", util::SymType{util::BuiltInType{util::BasicType::CHAR}});
  typetab_.insert("string", util::SymType{util::BuiltInType{util::BasicType::STRING}});
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
auto SemantContext::nowfunc() -> std::string
{
  if (functions_.empty()) {
    return "";
  }
  return functions_.top();
}

void SemantContext::pushfunc(const std::string &func_name)
{
  functions_.emplace(func_name);
}

void SemantContext::popfunc()
{
  functions_.pop();
}

void SemantContext::gen_error_msg(const parse::location &loc, const std::string &error_msg, const std::string &id)
{
  std::stringstream sstr;
  sstr << loc << ": "
       << error_msg << " " << id;
  error_msgs_.emplace_back(sstr.str());
}

}  // namespace pascc::semant