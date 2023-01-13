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
  TreeNode* deleteNode(TreeNode* root, int key) {
    if (root == NULL) {//base case if null then root is returned
        return root;
    }
    if (key > root->val) {//left side of root
      root->right = deleteNode(root->right, key);
    } else if (key < root->val) {//right side of root
      root->left = deleteNode(root->left, key);
    } else {
      if (root->left != NULL && root->right != NULL) {//if no child then
        
          TreeNode* min = root->right;//makes the right of the root the min to compare to the left
        
          while (min->left != NULL){//keeps checking the left msot node which is the lowest val
            min = min->left;//left node is now min
        }
          
        root->val = min->val;
        root->right = deleteNode(root->right, min->val);// the key is min val now 
      } else {
        TreeNode* n = root->left == NULL ? root->right : root->left;
        root->left = root->right = NULL;
        
          delete root;
        
          return n;
      }
    }    
    return root;
  }
};