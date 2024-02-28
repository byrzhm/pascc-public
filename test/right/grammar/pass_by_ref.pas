program PassByReferenceExample;

var
  a, b: integer;

procedure Swap(var x, y: integer);
var
  temp: integer;
begin
  temp := x;
  x := y;
  y := temp;
end;

begin
  a := 5;
  b := 10;
  
  writeln('Before swapping: a = ', a, ', b = ', b);
  
  // 通过引用调用 Swap 过程
  Swap(a, b);
  
  writeln('After swapping: a = ', a, ', b = ', b);
end.