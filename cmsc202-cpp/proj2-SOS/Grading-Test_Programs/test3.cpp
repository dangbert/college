/* File: test3.cpp
 *
 * Checks isSubset() method.
 */

#include <iostream>
#include <algorithm>
#include "sos.h"

using namespace std ;



void checkDump(vector<string> &A) ;

int main() {
   SoS kidsToys, iToys, bigKidsToys ;

   kidsToys.insert("Legos") ;
   kidsToys.insert("Xbox") ;
   kidsToys.insert("My Little Pony") ;
   kidsToys.insert("Pokemon Cards") ;

   iToys.insert("iPhone") ;
   iToys.insert("iPad") ;
   iToys.insert("iWatch") ;
   iToys.insert("iWantIt") ;

   SoS myToys = kidsToys.makeUnion(iToys) ;

   bigKidsToys.insert("BMW") ;
   bigKidsToys.insert("Porsche") ;
   bigKidsToys.insert("Tesla") ;
   bigKidsToys.insert("Horse") ;

   myToys = myToys.makeUnion(bigKidsToys) ;
   myToys = myToys.makeUnion(bigKidsToys) ;
   cout << "My Toys are:\n" ;
   myToys.print() ;
   cout << endl ;
   
   SoS wishList ;
   wishList.insert("Legos") ;
   wishList.insert("Horse") ;

   if (wishList.isSubset(myToys)) {
      cout << "I have my wish!\n" ;
   } else {
      cout << "You can't always get what you want" ;
   }

   wishList.insert("Bentley") ;

   if (wishList.isSubset(myToys)) {
      cout << "I have my wish!\n" ;
   } else {
      cout << "You can't always get what you want" ;
   }


   cout << "\n\n*** Dump Tests: ***\n\n" ;

   cout << "Dump kidsToys" ;
   vector<string> A1 = kidsToys.dump() ;
   checkDump(A1) ;

   cout << "Dump iToys" ;
   vector<string> A2 = iToys.dump() ;
   checkDump(A2) ;

   cout << "Dump bigKidsToys" ;
   vector<string> A3 = bigKidsToys.dump() ;
   checkDump(A3) ;

   cout << "Dump myToys" ;
   vector<string> A4 = myToys.dump() ;
   checkDump(A4) ;

   cout << "Dump wishList" ;
   vector<string> A5 = wishList.dump() ;
   checkDump(A5) ;
   
}
 

void checkDump(vector<string> &A) {
   sort(A.begin(), A.end()) ; 
   cout << endl ;
   for (unsigned int i=0 ; i < A.size() ; i++) {
      cout << "A[" << i << "] = " << A[i] << endl ;
   }
   cout << endl ;
}
