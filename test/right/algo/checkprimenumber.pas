program CheckPrimeNumber;

var
    num: Int64;
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
        j := 2;
        while (j <= Trunc(Sqrt(n))) and (n mod j <> 0) do
            inc(j); 

        if n mod j = 0 then
            IsPrime := false;
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