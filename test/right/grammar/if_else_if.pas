program IfElseIfExample;

var
    x: integer;

begin
    readln(x);
    if x < 0 then
        writeln('x is negative')
    else if x = 0 then
        writeln('x is zero')
    else
        writeln('x is positive');
end.