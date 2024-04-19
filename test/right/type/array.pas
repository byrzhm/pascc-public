program ArrayTest;
var
  a: array [0..9, 0..9] of integer;
begin
  a[0, 0] := 666;
  writeln(a[0, 0]);
  writeln(a[0][0]);
end.
