#include <iostream>
#include "Poly.h"

using namespace std;
void testDestructor();
void testInsert_And_Add();

int main(){
  //  testDestructor();
  testInsert_And_Add();
}

void testAdd(){
  Polynomial p5;

  Polynomial p6;

}

void testInsert_And_Add(){
  Polynomial p3;
  cout << "highest degree=" << p3.degree() << endl; //test degree function
  p3.insertMonomial(33, 3); //coeff, deg //test insert into empty list
  p3.insertMonomial(55,5);
  p3.insertMonomial(22, 2);
  p3.insertMonomial(88, 8);
  cout << "highest degree=" << p3.degree() << endl; //test degree function
  p3.insertMonomial(66, 6);
  
  cout << p3 << endl;

  cout << "highest degree=" << p3.degree() << endl;
  cout << endl << endl;

  Polynomial p4;
  p3.insertMonomial(1, 1);
  p4.insertMonomial(4, 4);
  p4.insertMonomial(5, 5);
  p4.insertMonomial(8, 8);
  p4.insertMonomial(2, 2);
  p4.insertMonomial(0, 0);

  cout << "preparing to add: " << p3 << "and " << p4;
  cout << p3.add(p4) << endl << endl;

  cout << "preparing to subtract: " << p3 << "and " << p4;
  cout << p3.subtract(p4) << endl << endl;

  Polynomial p5;
  p5.insertMonomial(69, 69);
  Polynomial p6 = p3.subtract(p4);

  cout << "evaluating: " << p4 << " for x=3" << endl;
  cout << "= " << p4.evaluate(3) << endl; //test evaluate function

  cout << "preparing to create a copy of " << p4 << endl;
  Polynomial p8(p4);
  cout << "copy: " << p8 << endl; 

  cout << "ADDing 20x^8 to: " << p4;
  p4.addTo(20, 8);
  cout << "result: " << p4;

  cout << "ADDing -2x^2 to: " << p4;
  p4.addTo(-2, 2);
  cout << "result: " << p4 << endl << endl;

  cout << "multiplying " << p3 << " and " << p6 << endl;
  cout << "result: "; 
  Polynomial p10 = p3.multiply(p6);
  cout << p10;
  cout << "at end of test code" << endl;
}

void testDestructor(){
  
  //deleting an empty polynomial
  Polynomial p1;
  
  //deleting a list with data:
  Polynomial p2;
  p2.insertMonomial(3, 3);
  p2.insertMonomial(4,4);
  //at the end of this block the destructors for the above polynomuials will automatically be called
}

