program TypeErrorExample;

var
  result: Integer;

function Add(a: Integer; b: Integer): Integer;
begin
  Add := a + b;
end;

begin
  result := Add(3, true);  // 将布尔值传递给整数类型的参数
end.