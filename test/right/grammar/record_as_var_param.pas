program RecordRefParamExample;

type
    Point = record
        x, y: integer;
    end;

var
    p: Point;

procedure Transform(var p: Point);
begin
    p.x := -p.x;
    p.y := -p.y;
end;

begin
    p.x := 10;
    p.y := 20;

    writeln('Old Point: ', p.x, ', ', p.y);
    Transform(p);
    writeln('New Point: ', p.x, ', ', p.y);
end.