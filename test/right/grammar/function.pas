program FunctionExample;

var
    a, b: Integer;

function Max(num1, num2: Integer): Integer;
var
    result: Integer;
begin
    if (num1 > num2) then
        result := num1
    else
        result := num2;
    Max := result;
end;

begin
    a := 100;
    b := 200;
    writeln('Max value is : ', Max(a, b));
end. 