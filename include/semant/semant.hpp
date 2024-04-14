#pragma once

#include "ast/visitor.hpp"
#include "semant/semant_context.hpp"

namespace pascc::semant {

class SemantVisitor: public Visitor
{
public:
  SemantVisitor() = default;

  SHARED_VISITOR_METHODS

  auto isOk() -> bool;

private:
  SemantContext context_;
};

}  // namespace pascc::semant
