class Solution {
 public:
  bool exist(vector<vector<char>>& board, string word) {
    for (int i = 0; i < board.size(); ++i)
      for (int j = 0; j < board[0].size(); ++j)
        if (dfs(board, word, i, j, 0))//call dfs to search
          return true;
    return false;
  }

 private:
  bool dfs(vector<vector<char>>& board, const string& word, int i, int j,
           int s) {
    if (i < 0 || i == board.size() || j < 0 || j == board[0].size())// base case 
      return false;
    if (board[i][j] != word[s] || board[i][j] == '0')//equlas to set val
      return false;
    if (s == word.length() - 1)//check for s update 
      return true;

    const char cur = board[i][j];
    board[i][j] = '0';
    const bool isExist = dfs(board, word, i + 1, j, s + 1) ||
                         dfs(board, word, i - 1, j, s + 1) ||
                         dfs(board, word, i, j + 1, s + 1) ||
                         dfs(board, word, i, j - 1, s + 1);
    board[i][j] = cur;

    return isExist;
  }
};
