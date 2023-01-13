/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* left;
    Node* right;
    Node* next;

    Node() : val(0), left(NULL), right(NULL), next(NULL) {}

    Node(int _val) : val(_val), left(NULL), right(NULL), next(NULL) {}

    Node(int _val, Node* _left, Node* _right, Node* _next)
        : val(_val), left(_left), right(_right), next(_next) {}
};
*/

class Solution {
public:
    Node* connect(Node* root) {
        
        if (!root || !root-> left)//if the left or root is null
            return root;
        
        root->left->next=root->right;
        // checks for the next node and sets it same as the left node pointer
        if (root->next)
            root->right->next=root->next->left;
        //recusrsion goes through same for the next nodes 
        connect(root->left);
        connect(root->right);
        
        return root;

    }
};