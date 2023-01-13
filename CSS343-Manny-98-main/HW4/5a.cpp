class Solution {
public:
  
    
    int minCostConnectPoints(vector<vector<int>>& ps) {
        
        int size = ps.size();
        int ans = 0;
        int i = 0;
        int conec = 0;//keeps track of connections
        vector<bool> visit(size);//if visited or not
        priority_queue<pair<int, int>> pq;//for ans
        
        while (++conec < size) {
            visit[i] = true;
            
            for (int j = 0; j < size; j++)
                
                if (!visit[j])
                    //int x =(abs(ps[i][0] - ps[j][0]) + abs(ps[i][1] - ps[j][1]));
                    //pq.push(x);
                    pq.push({-(abs(ps[i][0] - ps[j][0]) + abs(ps[i][1] - ps[j][1])), j});
            
            while (visit[pq.top().second])
                    
                    pq.pop();//part we dont need 
                    ans -= pq.top().first;
                    i = pq.top().second;//now the next point to compare
                    pq.pop();//rids of first element
            }
    
        return ans;
}
};