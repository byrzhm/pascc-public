#pragma once

#include <fstream>

#include "ast/visitor.hpp"

namespace pascc::codegen {

class CodegenVisitor: public Visitor
{
public:
  explicit CodegenVisitor(const std::string &filepath)
  {
    if (!filepath.empty()) {
      file_output_ = true;
      fout_.open(filepath);
    }
  }

  SHARED_VISITOR_METHODS

private:
  auto println(const std::string &str) -> void;
  auto printIndent() -> void;

  template<typename T>
  void print(const T &t);

  bool file_output_ = false;
  std::ofstream fout_;
  int indent_ = 0;
};

}  // namespace pascc::codegen
