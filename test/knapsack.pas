program KnapsackProblem;

const
  MAXN = 100;
  
var
  N, Capacity: integer;
  wt, val: array[1..MAXN] of integer;
  dp: array[0..MAXN, 0..MAXN] of integer;
  i, j: integer;

function max(a, b: integer): integer;
begin
  if a > b then
    max := a
  else
    max := b;
end;

begin
  // 读入物品数量和背包容量
  write('Enter number of items: ');
  readln(N);
  write('Enter the capacity of knapsack: ');
  readln(Capacity);
  
  // 读入每件物品的重量和价值
  writeln('Enter weight and value for each item:');
  for i := 1 to N do
  begin
    readln(wt[i], val[i]);
  end;
  
  // 初始化dp数组
  for i := 0 to N do
    dp[i, 0] := 0;
  for j := 0 to Capacity do
    dp[0, j] := 0;
  
  // 动态规划求解
  for i := 1 to N do
  begin
    for j := 1 to Capacity do
    begin
      if wt[i] <= j then
        dp[i, j] := max(dp[i-1, j], val[i] + dp[i-1, j-wt[i]])
      else
        dp[i, j] := dp[i-1, j];
    end;
  end;
  
  // 输出结果
  writeln('The maximum value that can be put in a knapsack of capacity ', Capacity, ' is: ', dp[N, Capacity]);
  
end.