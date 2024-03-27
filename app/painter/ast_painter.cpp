#include "util/painter.hpp"
#include "driver/driver.hpp"

using pascc::driver::Driver;
using pascc::util::PaintVisitor;

auto main() -> int
{
  PaintVisitor visitor("ast.dot");
  Driver::Get().parse("test.pas");
  Driver::Get().program().accept(visitor);
  return 0;
}