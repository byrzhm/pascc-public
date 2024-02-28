program CheckPrimeNumber;

var
    num, i: Int64;
    isNumPrime: boolean;

function IsPrime(n: Int64): boolean;
var
    j: Int64;
begin
    IsPrime := true;

    if n <= 1 then
        IsPrime := false
    else
    begin
        for j := 2 to Trunc(Sqrt(n)) do
        begin
            if n mod j = 0 then
            begin
                IsPrime := false;
                break;
            end;
        end;
    end;
end;

begin
    writeln('请输入一个整数：');
    readln(num);

    isNumPrime := IsPrime(num);

    if isNumPrime then
        writeln(num, ' 是质数。')
    else
        writeln(num, ' 不是质数。');

end.