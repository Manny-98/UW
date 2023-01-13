/*
// Definition for a Node.
class Node {
public:
    int val;
    vector<Node*> neighbors;
    Node() {
        val = 0;
        neighbors = vector<Node*>();
    }
    Node(int _val) {
        val = _val;
        neighbors = vector<Node*>();
    }
    Node(int _val, vector<Node*> _neighbors) {
        val = _val;
        neighbors = _neighbors;
    }
};
*/

class Solution {
public:
    Node* cloneGraph(Node* node) {
        if(!node)
        {
          return NULL;  
        } 
        
        map<Node*, Node*> cloneMap;
        queue<Node*> q;
        Node *newNode, *newNeighbor;
        
        q.push(node);
        
        while(!q.empty()) {//until empty
            Node* cur=q.front(); q.pop();
            if(cloneMap.find(cur) == cloneMap.end())//checks if target is at end
            {
                newNode=new Node(cur->val);
                cloneMap[cur]=newNode;//at pos add new node
            }
            else 
            {
                newNode=cloneMap[cur];
            }
            for(auto i:cur->neighbors) {//checks given neighbors
                if(cloneMap.find(i)==cloneMap.end()) 
                {
                    newNeighbor = new Node(i->val);
                    cloneMap[i]=newNeighbor;
                    q.push(i);//add to que
                }
                else {
                    newNeighbor = cloneMap[i];
                }
                (newNode->neighbors).push_back(newNeighbor);
            }
        }
        return cloneMap[node];//final map
    }
};