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

  bool in_field_designator_;
  bool field_is_ref_;

  std::string current_subprog_;
  util::SubprogType *current_subprog_type_;
};

}  // namespace pascc::codegen