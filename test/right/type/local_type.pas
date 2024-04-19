program LocalType;

type
  x = array [1 .. 10] of integer;

procedure MyProcedure();
type
  x = integer;
var
  y: x;
begin
  y := 1;
  writeln(y);
end;
    
begin
  MyProcedure
end.
