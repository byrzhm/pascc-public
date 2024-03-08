program FourQueens;

const
  N = 4; // 棋盘大小

var
  board: array[1..N, 1..N] of boolean; // 棋盘
  solutionCount: integer; // 解的计数器

procedure InitializeBoard;
var
  i, j: integer;
begin
  for i := 1 to N do
    for j := 1 to N do
      board[i, j] := False;
end;

procedure PrintBoard;
var
  i, j: integer;
begin
  for i := 1 to N do
  begin
    for j := 1 to N do
    begin
      if board[i, j] then
        write('Q ')
      else
        write('. ');
    end;
    writeln;
  end;
  writeln;
end;

function CanPlaceQueen(row, col: integer): boolean;
var
  i, j: integer;
begin
  // 检查当前列是否有皇后
  for i := 1 to row - 1 do
  begin
    if board[i, col] then
      Exit(False);
  end;

  // 检查左上对角线是否有皇后
  i := row - 1;
  j := col - 1;
  while (i >= 1) and (j >= 1) do
  begin
    if board[i, j] then
      Exit(False);
    i := i - 1;
    j := j - 1;
  end;

  // 检查右上对角线是否有皇后
  i := row - 1;
  j := col + 1;
  while (i >= 1) and (j <= N) do
  begin
    if board[i, j] then
      Exit(False);
    i := i - 1;
    j := j + 1;
  end;

  // 如果通过了上述检查，则可以放置皇后
  Exit(True);
end;

procedure SolveQueens(row: integer);
var
  col: integer;
begin
  if row > N then
  begin
    // 找到解，打印棋盘
    PrintBoard;
    solutionCount := solutionCount + 1;
    Exit;
  end;

  for col := 1 to N do
  begin
    if CanPlaceQueen(row, col) then
    begin
      // 在当前位置放置皇后
      board[row, col] := True;
      // 递归解决下一行
      SolveQueens(row + 1);
      // 回溯，尝试其他位置
      board[row, col] := False;
    end;
  end;
end;

var x : integer;

begin
  x := 1;
  writeln('x = ', x);
  // 初始化棋盘
  InitializeBoard;
  // 求解四皇后问题
  solutionCount := 0;
  SolveQueens(1);
  writeln('Total solutions: ', solutionCount);
end.