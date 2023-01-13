class Trie {
private:
    class TrieNode{//struct for TrieNode
        public:
        vector<TrieNode*> abc;//vector that contais alphabets
        bool end;
        TrieNode(){//new object
            abc.resize(26,nullptr);
            end = false;
        }
        };
public:
    TrieNode* root;
    Trie() {
        root = new TrieNode();
    }
    
    void insert(string word) {//itterate to insert
        TrieNode* node = root;
        for(int i=0;i<word.size();i++){
            if(node->abc[word[i]-'a'] == NULL){//
                node->abc[word[i]-'a'] = new TrieNode();//new node if null found
            }
            node = node->abc[word[i]-'a'];
        }
        node->end = true;//marks end of node
    }
    
    bool search(string word) {//itterate to search
        TrieNode* node = root;
        
        for(int i=0;i<word.size();i++){
            if(node->abc[word[i]-'a'] == NULL)
                return false;
            node = node->abc[word[i]-'a'];
        }
        return node->end;
    }
    
    bool startsWith(string prefix) {
        TrieNode* node = root;
        for(int i=0;i<prefix.size();i++){
            if(node->abc[prefix[i]-'a'] == nullptr)
                return false;
            node = node->abc[prefix[i]-'a'];
        }
        return true;
    }
};

/**
 * Your Trie object will be instantiated and called as such:
 * Trie* obj = new Trie();
 * obj->insert(word);
 * bool param_2 = obj->search(word);
 * bool param_3 = obj->startsWith(prefix);
 */