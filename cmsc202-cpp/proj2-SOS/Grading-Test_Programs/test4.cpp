/* File: test4.cpp
 *
 * Checks makeUnion() method.
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
   SoS notMyToys = myToys.makeComplement() ;

   bigKidsToys.insert("BMW") ;
   bigKidsToys.insert("Porsche") ;
   bigKidsToys.insert("Tesla") ;
   bigKidsToys.insert("Horse") ;

   SoS hisToys = bigKidsToys.makeUnion(iToys) ;
   SoS notHisToys = hisToys.makeComplement() ;

   SoS set1 = myToys.makeUnion(notHisToys) ;
   cout << "\nSet #1\n" ;
   set1.print() ;

   SoS set2 = notMyToys.makeUnion(notHisToys) ;
   cout << "\nSet #2\n" ;
   set2.print() ;

   SoS set3 = notMyToys.makeUnion(hisToys) ;
   cout << "\nSet #3\n" ;
   set3.print() ;

   SoS set4 = myToys.makeUnion(hisToys) ;
   cout << "\nSet #4\n" ;
   set4.print() ;
   
   SoS set5 = hisToys.makeUnion(notMyToys) ;
   cout << "\nSet #5\n" ;
   set5.print() ;

   SoS set6 = notHisToys.makeUnion(notMyToys) ;
   cout << "\nSet #6\n" ;
   set6.print() ;

   SoS set7 = notHisToys.makeUnion(myToys) ;
   cout << "\nSet #7\n" ;
   set7.print() ;

   SoS set8 = hisToys.makeUnion(myToys) ;
   cout << "\nSet #8\n" ;
   set8.print() ;

   SoS all = myToys.makeUnion(notMyToys) ;
   cout << "\nEverything:\n" ;
   all.print() ;

   SoS none = all.makeComplement() ;
   cout << "\nNothing:\n" ;
   none.print() ;
}
 
