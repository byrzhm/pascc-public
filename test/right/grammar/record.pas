program RecordExample;

type
    Point = record
        x, y: Integer;
    end;

var
    p: Point;

begin
    p.x := 10;
    p.y := 20;
    WriteLn(p.x, ' ', p.y);
end.