#include <iostream>
#include <algorithm>
#include "sos.h"
using namespace std ;


int main() {
   SoS commonPets ;

   commonPets.insert("hamster") ; 
   commonPets.insert("cat") ; 
   commonPets.insert("dog") ; 
   commonPets.insert("fish") ; 

   cout << "Some common pets:\n" ;
   commonPets.print() ;

   cout << endl ;

   if (commonPets.isMember("dog")) 
      cout << "Dogs are common pets\n" << endl ;

   if (! commonPets.isMember("iguana")) 
      cout << "Iguanas are not common pets\n" << endl ;

   vector<string> A = commonPets.dump() ;
   sort(A.begin(), A.end());
   cout << "\n***Data dump:\n" ;
   for (unsigned int i=0 ; i < A.size() ; i++) {
      cout << "A[" << i << "] = " << A[i] << endl ;
   }

}
 
