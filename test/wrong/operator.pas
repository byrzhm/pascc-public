program ModError;

begin
  writeln(10 / 0); // OK
  writeln(10 div 0);
  writeln(10 div 1.1);
  writeln(10 mod 0);
  writeln(10 mod 1.1);
end.
