#pragma once

#include "util/symtab.hpp"
#include "util/type/type.hpp"

namespace pascc::codegen {

class CodegenContext
{
  friend class CodegenVisitor;

public:
  CodegenContext();

  void enterScope();

  void exitScope();

private:
  util::SymTab<std::string, util::SymType *> typetab_;
  util::SymTab<std::string, std::string> consttab_;
  util::SymTab<std::string, util::VarType *> vartab_;
  util::SymTab<std::string, util::SubprogType *> subprogtab_;

  bool build_format_string_;
};

}  // namespace pascc::codegen