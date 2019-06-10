#include <iostream>
#include <algorithm>
#include "sos.h"

using namespace std ;



void checkDump(vector<string> &A) ;

int main() {
   SoS commonPets ;

   commonPets.insert("hamster") ; 
   commonPets.insert("cat") ; 
   commonPets.insert("dog") ; 
   commonPets.insert("fish") ; 
   commonPets.insert("rabbit") ; 

   SoS dangerous ;
   dangerous.insert("lion") ;
   dangerous.insert("tiger") ;
   dangerous.insert("rabbit") ;  // nasty, big, pointy teeth
   dangerous.insert("bear") ;
   dangerous.insert("dog") ;


   // test complementation
   SoS notCommonPets = commonPets.makeComplement() ;
   cout << "These animals are not common pets:\n" ;
   notCommonPets.print() ;


   // test finite intersect cofinite
   SoS dangerousAndNotCommon = dangerous.makeIntersection(notCommonPets) ;
   cout << "\n\nThese animals are unusual and are not common as pets:\n" ;
   dangerousAndNotCommon.print() ;

   // test finite intersect finite
   SoS badPets = commonPets.makeIntersection(dangerous) ;
   cout << "\n\nPeople should not have these pets:\n" ;
   badPets.print() ;

   // test cofinite intersect finite
   SoS safe = dangerous.makeComplement() ;
   SoS goodPets = safe.makeIntersection(commonPets) ;
   cout << "\n\nPeople should have one of these pets:\n" ;
   goodPets.print() ;

   // test cofinite intersect cofinite
   SoS weirdSafe = safe.makeIntersection(notCommonPets) ;
   if ( weirdSafe.isMember("iguana") ) {
      cout << "\nIguanas are not common but also not dangerous.\n" ;
   }

   vector<string> A1 = commonPets.dump() ;
   checkDump(A1) ;
   vector<string> A2 = dangerous.dump() ;
   checkDump(A2) ;
   vector<string> A3 = notCommonPets.dump() ;
   checkDump(A3) ;
   vector<string> A4 = dangerousAndNotCommon.dump() ;
   checkDump(A4) ;
   vector<string> A5 = badPets.dump() ;
   checkDump(A5) ;
   vector<string> A6 = goodPets.dump() ;
   checkDump(A6) ;
   vector<string> A7 = weirdSafe.dump() ;
   checkDump(A7) ;
}
 

void checkDump(vector<string> &A) {
   sort(A.begin(), A.end()) ; 
   cout << endl ;
   for (unsigned int i=0 ; i < A.size() ; i++) {
      cout << "A[" << i << "] = " << A[i] << endl ;
   }
   cout << endl ;
}
