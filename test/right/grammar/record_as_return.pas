program RecordReturnExample;

type
    Point = record
        x, y: Integer;
    end;

var
    p: Point;

function CreatePoint(x, y: Integer): Point;
begin
    CreatePoint.x := x;
    CreatePoint.y := y;
end;

begin
    p := CreatePoint(10, 20);
    WriteLn(p.x, ' ', p.y);
end.