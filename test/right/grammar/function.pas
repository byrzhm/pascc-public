program FunctionExample;

var
    a, b: Integer;

function Max(num1, num2: Integer): Integer;
begin
    if (num1 > num2) then
        Max := num1
    else
        Max := num2;
end;

begin
    a := 100;
    b := 200;
    writeln('Max value is : ', Max(a, b));
end. 