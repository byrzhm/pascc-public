program InsertionSortExample;

const
  MAX = 100;

type
  TArray = array[1..MAX] of Integer;

var
  A: TArray;
  I: Integer;
  Size: Integer;

procedure InsertionSort(var A: TArray; Size: Integer);
var
  I, J, Key: Integer;
begin
  for I := 2 to Size do
  begin
    Key := A[I];
    J := I - 1;

    while (J >= 1) and (A[J] > Key) do
    begin
      A[J + 1] := A[J];
      Dec(J);
    end;

    A[J + 1] := Key;
  end;
end;

begin
  Readln(Size);
  for I := 1 to Size do
  begin
    Read(A[I]);
  end;

  { Display unsorted array }
  {writeln('Unsorted array:');
  for I := 1 to Size do
    write(A[I], ' ');
  writeln;}

  { Sort array }
  InsertionSort(A, Size);

  { Display sorted array }
  {writeln('Sorted array:');}
  for I := 1 to Size do
    write(A[I], ' ');
  writeln;
end.