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

  [[nodiscard]] auto error_msgs() const -> const std::vector<std::string> &;

private:
  /**
   * @brief and or
   */
  void visitLogicBinaryExpr(ast::BinaryExpr &node);

  /**
   * @brief = <> < > <= >=
   */
  void visitComparisonBinaryExpr(ast::BinaryExpr &node);

  /**
   * @brief + - *
   */
  void visitArithmeticBinaryExpr(ast::BinaryExpr &node);

  /**
   * @brief /
   */
  void visitFDivBinaryExpr(ast::BinaryExpr &node);

  /**
   * @brief div mod
   */
  void visitIntOpBinaryExpr(ast::BinaryExpr &node);

  SemantContext context_;
};

}  // namespace pascc::semant
