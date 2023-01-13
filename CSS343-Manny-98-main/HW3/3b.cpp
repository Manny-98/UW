class Solution {
 public:
  int numIslands(vector<vector<char>>& grid) {
    int ans = 0;

    for (int i = 0; i < grid.size(); ++i)//it through grid given
      for (int j = 0; j < grid[0].size(); ++j)
        if (grid[i][j] == '1') {
          dfs(grid, i, j);
          ++ans;
        }

    return ans;
  }

 private:
  void dfs(vector<vector<char>>& grid, int i, int j) {//dfs search 
    if (i < 0 || i == grid.size() || j < 0 || j == grid[0].size())
      return;
    if (grid[i][j] != '1')
      return;

    grid[i][j] = 'x';  // mark as visited
    dfs(grid, i + 1, j);//checks adjacent recursivly
    dfs(grid, i - 1, j);
    dfs(grid, i, j + 1);
    dfs(grid, i, j - 1);
  }
};