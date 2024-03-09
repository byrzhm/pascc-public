#pragma once

namespace pascc::semant {

class SemantContext
{
public:
  SemantContext() = default;

  [[nodiscard]] auto error_count() const -> int { return error_count_; }

private:
  int error_count_ = 0;
};

}  // namespace pascc::semant