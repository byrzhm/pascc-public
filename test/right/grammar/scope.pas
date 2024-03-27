program ScopeExample;

var
    x: Integer;

procedure AddOne;
begin
    x := x + 1;
end;

begin
    x := 0;
    AddOne;
    WriteLn(x);
end.