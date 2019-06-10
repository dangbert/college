#include <iostream>
#include <cstring>
using namespace std; //needed for strlen()

const int size=80; //size of str
void ModifyString(char str[]);

int main()
{
  char str[size];
  cout << "Enter a string: " << endl;
  cin.getline(str, sizeof(str));
  
  ModifyString(str);

  cout << "The modified string is: " << str << endl;


  return 0;
}

void ModifyString(char str[]) //converts to lowercase and underscore
{
  for(int i=0; str[i] != '\0'; i++)
    {
      if(str[i] == ' ')
	str[i]='_';
      if(str[i]>=97)
	str[i]=str[i]-32;
    }


}
