#include <iostream>
#include <cstring>
#include "proj1.h"
using namespace std;

bool interesting(const char msg[]);
void ConvertConstArray(const char ar1[], char ar2[]);
void PrintArray(const char ar[]);
void ClearArray(char ar[]);


int main()
{
char ans[MAXMSGLEN];            //array to hold a message after it's been decrypted

for(int n=0; n<NUMMSGS; n++)    //loop through all ciphers
   {
   SolveCipher(cipher[n], ans); //overwrite ans with the decrypted version of cipher[n]
   
   cout << "Message #" << n+1;  //add 1 so the first cipher (cipher[0]) is called Message #1
   if(interesting(ans))         //if ans starts with a crib
      {
      cout << ", key " << SolveCipher(cipher[n], ans) << endl; //print the correct key used to decrypt
      PrintArray(ans);          //print the decrypted array
      }
   else   
      cout << " was not interesting." << endl; //inidicate the array did not start with a crib
   cout << endl;
   }
}
   
   
char SolveCipher(const char cip[], char dec[]) //returns the key if the ciphered message starts with a crib and overwrites dec with the deiphered message
   {
   ClearArray(dec);
   for(char k='A'; k<='Z'; k++)    //loop through all possible keys
      {
      ConvertConstArray(cip, dec); //copy cip to dec because dec is writable
      Decipher(dec, k);            //updates dec to be the deciphered version of cip using key k
      if(interesting(dec))         //return the key if the cipher decrypted with this key starts with a crib
         return k;
      }
   return '\0';
   }
   
   
void Decipher(char cip[], char key)
   {
   for(int i=0; cip[i] != '\0'; i++)
      {
         if(cip[i] != ' ') //only decipher a char if it's not a space
            cip[i] = ( (int) cip[i] - (int) key + 26 ) % 26 + 65; /*subtract the key from the current character, add 26 and take the mod of the result
                                                                    to ensure it's between 0 and 26. Then add 65 to make it a letter A-Z */
      }   
   }


//returns true if a decrypted cypher msg begins with a crib; returns false otherwise
bool interesting(const char msg[])
   {
   for(int n=0; n<NUMCRIBS; n++) //cycle through all the cribs
      {
      for(int i=0; cribs[n][i] != '\0'; i++) //cycle through all the characters of a particular crib
         {
         if(cribs[n][i] != msg[i]) //compare each char of the current crib to the correcponding char location in msg
            break; //move to the next n because the message doesn't start with this particular crib
         
         if(cribs[n][i+1] == '\0') //if this was the last letter of i then msg[] started with this crib
            return true; //return true becaues msg starts with a crib
         }
      }
   return false; //return false if msg was never found to start with a crib
   }
   

//copies the elements of a const array ar to a writable array ar2
void ConvertConstArray(const char ar1[], char ar2[])
   {
   ClearArray(ar2); //wipe ar2 to ensure it's empty
   for(int i=0; ar1[i] != '\0'; i++)
      {
      ar2[i] = ar1[i]; //copy the elements of ar2 into ar1
      }
   }


//clear all elements in an array ar
void ClearArray(char ar[])
   {
   for(int i=0; ar[i] != '\0'; i++) //i<MAXMSGLEN; i++)
      {
      ar[i] = '\0'; //clear ar by replacing all its characters with '\0'
      }
   }

//print all elements of an array ar on a single line. (ar doesn't have to be const, but this function promises not to modify it anyway)
void PrintArray(const char ar[])
   {
   for(int i=0; ar[i] != '\0'; i++)
      cout << ar[i];
   cout<< endl;
   }