program PassByReferenceExample;

var
  a: Integer;

procedure Foo(var x: Integer);
begin
  x := 42;
end;

procedure Bar(var x: Integer);
begin
  Foo(x);
end;

begin
    Bar(a);
    WriteLn(a);
end.