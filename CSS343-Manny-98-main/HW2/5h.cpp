struct comparison{//defination of cemparator
    bool operator()(const vector<int>& p1, const vector<int>& p2 ){
        return (p1[0]*p1[0] + p1[1]*p1[1])<(p2[0]*p2[0] + p2[1]*p2[1]);
    }
};

class Solution {
public:
    vector<vector<int>> kClosest(vector<vector<int>>& points, int k) {
        priority_queue<vector<int>, vector<vector<int>>, comparison> prq;//priority que
        for(vector<int>& pt : points){//amount of points given 
            if(prq.size()<k)//case if less points than k
                prq.push(pt);
            else{
                const vector<int>& top=prq.top();
                int dist1=top[0]*top[0]+top[1]*top[1];//finds dist
                int dist2=pt[0]*pt[0]+pt[1]*pt[1];//finds dist 
                if(dist1>dist2){//adds to queue 
                    prq.pop();
                    prq.push(pt);
                }
            }
        }
        vector<vector<int>> ans;
        while(!prq.empty()){//populates vector 
            ans.push_back(prq.top());
            prq.pop();
        }
        return ans;
        
        
    }
};