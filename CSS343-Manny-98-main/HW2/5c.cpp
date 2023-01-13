/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
        TreeNode* buildTree_rec(vector<int>& inorder,int ind1, int ind2, vector<int>& postorder, int pst1,int pst2) {//ind1 and ind2 and pst2 pst1 keep track of the indexes
        if(ind1>=ind2 || pst1>=pst2)//checks for the end
            return nullptr;
            
            TreeNode *root = new TreeNode(postorder[pst2-1]);//makes the last element in the post order traversal the root
            
            auto it = find(inorder.begin() + ind1, inorder.begin() + ind2, postorder[pst2-1]); //finds the root in the inorder at index the root was found in post order
            int diff = it - inorder.begin()-ind1;
            root->left = buildTree_rec(inorder,ind1,ind1+diff,postorder,pst1,pst1+diff);//recursivley builds the left 
            root->right =buildTree_rec(inorder,ind1+diff+1,ind2,postorder,pst1+diff,pst2-1);//builds right
        return root;
    }
public:
    
    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
        int n=inorder.size();
        if(inorder.size()==0){//if no elements return null
           return nullptr;
            }
               return buildTree_rec(inorder,0,n,postorder,0,n);
        
        
    }
};