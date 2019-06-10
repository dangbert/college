

#include <iostream>
#include "List7.h"

int main() {

   List List1;
   List List2;



   cout << "Testing Insert..." << endl;
   for(int i = -5; i < 10; i+=2) {
      List1.insert(i);
   }

   for(int i = 6; i > -11; i -= 5) {
      List1.insert(i);
   }

   for(int i = -7; i < 11; i+= 3) {
      List1.insert(i);
   }

   for(int i = 2; i > -2; i--) {
      List1.insert(i);
   }

   cout << "Should Print: -9 -7 -5 -4 -4 -3 -1 -1 -1 0 1 1 1 2 2 3 5 5 6 7 8 9" << endl;
   cout << "Actual Print: ";
   List1.Print();
   cout << endl << endl;



   cout << "Testing Remove..." << endl;
   
   cout << "Remove 5..." << endl;
   List1.remove(5);
   cout << "Should Print: -9 -7 -5 -4 -4 -3 -1 -1 -1 0 1 1 1 2 2 3 5 6 7 8 9" << endl;
   cout << "Actual Print: ";
   List1.Print();
   cout << endl << endl;

   cout << "Remove -1..." << endl;
   List1.remove(-1);
   cout << "Should Print: -9 -7 -5 -4 -4 -3 -1 -1 0 1 1 1 2 2 3 5 6 7 8 9" << endl;
   cout << "Actual Print: ";
   List1.Print();
   cout << endl << endl;

   cout << "Remove 9..." << endl;
   List1.remove(9);

   cout << "Should Print: -9 -5 -4 -4 -3 -1 -1 -1 0 1 1 1 2 2 3 5 6 7 8" << endl;
   cout << "Actual Print: ";
   List1.Print();
   cout << endl << endl;

   return 0;

}
