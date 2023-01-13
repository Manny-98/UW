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
    TreeNode* insertIntoBST(TreeNode* root, int val) {
        
        TreeNode *n = new TreeNode (val);
        if (root==NULL){//if nothing exists resturns the val as the root
            return n;
        }
        if (root->val<val){//if val> the root recursivly calls the insert functions
            root->right= root->right ? insertIntoBST(root->right, val):n;
        }else{//if val< the root recursivly calls the insert functions
            root->left = root->left ? insertIntoBST(root->left, val):n;
        }
        return root;
    }
};