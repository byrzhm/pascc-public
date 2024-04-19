program RecordParamExample;
type
    Point = record
        x, y: Integer;
    end;

var
    p: Point;

procedure PrintPoint(p: Point);
begin
    writeln(p.x, ' ', p.y);
end;

begin
    p.x := 10;
    p.y := 20;
    PrintPoint(p);
end.