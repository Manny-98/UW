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
private:    
    TreeNode* preNode = NULL;//tracks the previous node
    
    bool check(TreeNode* node) {
        if (node == NULL){ //if null its true
            return true;
        }
        
        if (!check(node->left)){ //recursivley checks left
            return false;
        }
        
        if (preNode != NULL && preNode->val >= node->val){ //checks is the preNode value has been updated and if its bigger than the node value
            return false;
        }
        
        preNode = node;//updates preNode value
        
        return check(node->right);//recursively checks the right node
    }
    
public:
    bool isValidBST(TreeNode* root) {
       
        return check(root);//helper function
    }
};