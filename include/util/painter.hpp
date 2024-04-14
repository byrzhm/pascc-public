#pragma once

#include <fstream>

#include "ast/visitor.hpp"

namespace pascc::util {

class PaintVisitor: public Visitor
{
public:
  PaintVisitor() = delete;

  explicit PaintVisitor(const std::string &filename)
    : out_(filename)
  {}

  SHARED_VISITOR_METHODS

private:
  std::ofstream out_;
};

}  // namespace pascc::util