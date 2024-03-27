program ScopeErrorExample;

procedure PrintNumber;
var
    temp: integer;
begin
    temp := 42;
    writeln(temp);
end;

begin
    PrintNumber;
    writeln(temp);
end.