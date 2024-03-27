#pragma once

#include <fstream>

void write_file(const std::string &filename, const std::string &content)
{
  std::ofstream file(filename);
  file << content;
}

auto write_basic_test_file() -> std::string
{
  const std::string content = R"(program basic;

(*
  This is a basic program.
*)

var
  x: integer;

begin
  x := 1;
  writeln(x);
end.

)";
  std::string filename      = "basic.pas";
  write_file(filename, content);
  return filename;
}
