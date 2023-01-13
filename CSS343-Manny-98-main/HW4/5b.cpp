class Solution {
public:
    bool isIsomorphic(string s, string t) {
        
        unordered_map<char,char>map1;
        unordered_set<char>map2;
        
        if(s.length()!=t.length())//if lenght not eqaul then return false
            return false;
        
        for(int i=0;i<s.length();i++){//it through the string 
           
            char char1=s[i];//set char while it thought string
            char char2=t[i];
            
            if(map1.find(char1)!=map1.end()){// not qual to end
                if(map1[char1]!=char2)
                    return false;
            }
            
            else{
                if(map2.find(char2)!=map2.end())
                    return false;
                
                map2.insert(char2);
                map1[char1]=char2;
            }
        }
        return true;
    }
};