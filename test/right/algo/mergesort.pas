program MergeSortExample;

const
  MAX = 100;

type
  TArray = array[1..MAX] of Integer;

var
  A: TArray;
  I: Integer;
  Size: integer;

procedure Merge(var A: TArray; Left, Middle, Right: Integer);
var
  I, J, K: Integer;
  N1, N2: Integer;
  L, R: TArray;
begin
  N1 := Middle - Left + 1;
  N2 := Right - Middle;

  for I := 1 to N1 do
    L[I] := A[Left + I - 1];

  for J := 1 to N2 do
    R[J] := A[Middle + J];

  I := 1;
  J := 1;
  K := Left;

  while (I <= N1) and (J <= N2) do
  begin
    if L[I] <= R[J] then
    begin
      A[K] := L[I];
      Inc(I);
    end
    else
    begin
      A[K] := R[J];
      Inc(J);
    end;
    Inc(K);
  end;

  while I <= N1 do
  begin
    A[K] := L[I];
    Inc(I);
    Inc(K);
  end;

  while J <= N2 do
  begin
    A[K] := R[J];
    Inc(J);
    Inc(K);
  end;
end;

procedure MergeSort(var A: TArray; Left, Right: Integer);
var
  Middle: Integer;
begin
  if Left < Right then
  begin
    Middle := (Left + Right) div 2;
    MergeSort(A, Left, Middle);
    MergeSort(A, Middle + 1, Right);
    Merge(A, Left, Middle, Right);
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
  for I := 1 to MAX do
    write(A[I], ' ');
  writeln;}

  { Sort array }
  MergeSort(A, 1, Size);

  { Display sorted array }
  {writeln('Sorted array:');}
  for I := 1 to Size do
    write(A[I], ' ');
  writeln;
end.