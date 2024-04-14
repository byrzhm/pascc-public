program TypeCast;

var
    i: integer;
    j: integer;

function foo(x: real; y: real): real;
begin
    foo := x + y;
end;

begin
    i := 1;
    j := 2;
    writeln(foo(i, j));
end.