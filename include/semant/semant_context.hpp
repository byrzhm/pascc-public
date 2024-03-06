#pragma once

namespace pascc::semant {

class SemantContext
{
public:
  static auto Get() -> SemantContext &;

private:
  SemantContext() = default;
};

}  // namespace pascc::semant