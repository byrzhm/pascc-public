program OutOfBoundExample;

var
  arr: array[1..6] of integer;

begin
  //  Warning: range check error while evaluating 
  //  constants (10 must be between 1 and 6)
  
  arr[10] := 1
end.
