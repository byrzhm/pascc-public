program KMP;

const
  MAX_N = 1000;

var
  T, P: string;
  N, M: integer;
  F: array[1..MAX_N] of integer;

procedure BuildFailureFunction(P: string; var F: array of integer);
var
  m, i, j: integer;
begin
  m := Length(P);
  F[1] := 0;
  j := 0;
  for i := 2 to m do
  begin
    while (j > 0) and (P[j + 1] <> P[i]) do
      j := F[j];
    if P[j + 1] = P[i] then
      Inc(j);
    F[i] := j;
  end;
end;

procedure KMP(T, P: string; var F: array of integer);
var
  n, m, i, j: integer;
begin
  n := Length(T);
  m := Length(P);
  BuildFailureFunction(P, F);
  i := 1;
  j := 1;
  while i <= n do
  begin
    if T[i] = P[j] then
    begin
      if j = m then
      begin
        writeln('Pattern found at position: ', i - m + 1);
        j := F[j];
      end
      else
        Inc(j);
      Inc(i);
    end
    else if j > 1 then
      j := F[j]
    else
      Inc(i);
  end;
end;

begin
  writeln('Enter the text string:');
  readln(T);
  writeln('Enter the pattern string to search:');
  readln(P);
  N := Length(T);
  M := Length(P);
  KMP(T, P, F);
end.
