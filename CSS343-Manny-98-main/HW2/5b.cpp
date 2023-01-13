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
public:
    void flatten(TreeNode* root) {
        
        if(root==NULL) //checks for root being null
            return;
        
        if(root->left==NULL)//checks if left child is NULL and continues to flatten the right via recurison
        {
            flatten(root->right);
            return;
        }
        
        TreeNode* lNode = root->left;
        TreeNode* rNode = root->right;
        
        flatten(root->left);//flattens the left subchilds nodes if any
        flatten(root->right);//flattens the rights subchilds nodes if any
        
        root->right =lNode;
        root->left=NULL;
        
        while(lNode->right!=NULL)// while there is a right subchidl to the right node
            lNode=lNode->right;
            lNode->right=rNode;
        
    }
};