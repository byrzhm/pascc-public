#pragma once


#include "location.hh"
#include "util/symtab.hpp"
#include "util/type/type.hpp"
#include "util/type/type_comparator.hpp"
#include <stack>


namespace pascc::semant {

class SemantContext
{
  friend class SemantVisitor;

public:
  SemantContext();

  void enterScope();

  void exitScope();

  auto topFunc() -> std::string;

  void pushFunc(const std::string &func_name);

  void popFunc();

  void genErrorMsg(const parse::location &loc, const std::string &error_msg, const std::string &id = "");


private:
  std::vector<std::string> error_msgs_;

  util::SymTab<std::string, util::SymType *> typetab_;
  util::SymTab<std::string, util::SymType *> consttab_;
  util::SymTab<std::string, util::VarType *> vartab_;
  util::SymTab<std::string, util::SubprogType*> subprogtab_;

  util::SubprogType::FormalVector formal_params_;
  std::stack<std::string> functions_;
  util::SymType *case_stmt_type_{nullptr};

  util::TypeComparator cmp_;
};

}  // namespace pascc::semant