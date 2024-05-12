#include "util/painter.hpp"
#include "driver/driver.hpp"

using pascc::driver::Driver;
using pascc::util::PaintVisitor;

auto main() -> int
{
  PaintVisitor visitor("ast.dot");
  Driver::get().parse("test.pas");
  Driver::get().program().accept(visitor);
  return 0;
}