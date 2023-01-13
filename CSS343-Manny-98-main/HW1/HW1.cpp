#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;

int main(int argc, char** argv)
{
  //converts the char to an integer
  int val= atoi(argv[1]);
  
  //input command stores the digits as a string
  cout<<"Enter 9 digits with no space: "<<endl;
  string digits;
  cin>>digits;
  
  //checks if the lenght of the string digits is exactly 9
  if(digits.length()!=9){
    cout<<"Invalid length of string!!"<<endl;
    return 0;
  }
  //pre stored numbers as letters
  string numbers[10]={"first","second","third", "fourth","fifth","sixth","seventh","eighth","ninth","tenth"};
    //itterates for val and prints the 3X3 box accordingly
    for(int i=0; i<val;i++){
      
      //keeps count of the iteration
      cout<<endl;
      cout<<endl;
      cout<<"The "<<numbers[i] <<" shuffled output is:"<<endl;
      //randomly shuffles the chars in the string digits
      random_shuffle(digits.begin(), digits.end());
      
      //Prints the box as per the requirement
      cout<<endl;
      cout<<endl;
      cout<<"+---------+"<<endl;
      cout<< "| "<<digits[0]<< " ";
      cout<< " "<<digits[1]<< " ";
      cout<< " "<<digits[2]<< " |"<<endl;
      cout<< "| "<<digits[3]<< " ";
      cout<< " "<<digits[4]<< " ";
      cout<< " "<<digits[5]<< " |"<<endl;
      cout<< "| "<<digits[6]<< " ";
      cout<< " "<<digits[7]<< " ";
      cout<< " "<<digits[8]<< " |"<<endl;
      cout<<"+---------+"<<endl;
      cout<<endl;
      cout<<endl;
      }
     return 0;
}
