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
  auto print(const std::string &str) -> void;

  bool file_output_ = false;
  std::ofstream fout_;
  int indent_ = 0;
};

}  // namespace pascc::codegen
