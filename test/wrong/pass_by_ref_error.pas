program PassByRefError;

procedure Foo(var x: Integer);
begin
  x := x + 1;
end;

begin
  Foo(10);
end.