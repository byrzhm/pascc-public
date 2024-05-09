program QuickPow;

var
  base, exponent, answer: integer;

function QuickPow(base, exponent: integer): integer;
begin
  if exponent = 0 then
    answer := 1
  else if exponent mod 2 = 0 then begin
    answer := QuickPow(base, exponent div 2);
    answer := answer * answer;
  end
  else begin
    answer := base * QuickPow(base, exponent - 1);
  end;
  QuickPow := answer;
end;

begin
  readln(base);
  readln(exponent);
  answer := QuickPow(base, exponent);
  writeln(answer);
end.
