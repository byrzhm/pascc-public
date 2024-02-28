program QuickSort;

const
  MAX_SIZE = 100;

type
  TArray = array [1..MAX_SIZE] of Integer;

var
  arr: TArray;
  size: Integer;
  i: Integer;

procedure Swap(var a, b: Integer);
var
  temp: Integer;
begin
  temp := a;
  a := b;
  b := temp;
end;

procedure QuickSort(var arr: TArray; low, high: Integer);
var
  pivot, i, j: Integer;
begin
  if low < high then
  begin
    pivot := arr[low];
    i := low + 1;
    j := high;

    while i <= j do
    begin
      while (i <= high) and (arr[i] < pivot) do
        Inc(i);
      while (j >= low) and (arr[j] > pivot) do
        Dec(j);

      if i < j then
      begin
        Swap(arr[i], arr[j]);
        Inc(i);
        Dec(j);
      end;
    end;

    Swap(arr[low], arr[j]);

    QuickSort(arr, low, j - 1);
    QuickSort(arr, j + 1, high);
  end;
end;

procedure PrintArray(const arr: TArray; size: Integer);
var
  i: Integer;
begin
  for i := 1 to size do
    Write(arr[i], ' ');
  Writeln;
end;

begin
  {Write('Enter the size of the array: ');}
  Readln(size);

  {Write('Enter elements of the array: ');}
  for i := 1 to size do
    Read(arr[i]);

  {Writeln('Original array:');}
  {PrintArray(arr, size);}

  QuickSort(arr, 1, size);

  {Writeln('Sorted array:');}
  PrintArray(arr, size);
end.