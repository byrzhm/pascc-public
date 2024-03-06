program ForLoopExample;

var
    i: Integer;

begin
    for i := 1 to 10 do
        WriteLn(i);

    for i := 10 downto 1 do
    begin
        WriteLn(i)
    end;
end.