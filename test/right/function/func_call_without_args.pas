program FuncCall;

function GetOne: Integer;
begin
    GetOne := 1;
end;

begin
    WriteLn(GetOne);
    WriteLn(GetOne());
end.