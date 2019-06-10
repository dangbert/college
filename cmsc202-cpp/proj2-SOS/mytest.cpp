#include <iostream>
#include "sos.h"

using namespace std;
void testMakeUnion(SoS &A, SoS &B);


int main()
   {   
   SoS s;// = new SoS();  //s = new Sos; // SoS s(); //create an empty finite set s
   s.print(); //test that printing an empty vector has no errors
   s.insert("s1"); //add elements to the vector
   s.insert("s2");
   s.insert("s3");
   s.insert("s4");
   s.insert("s4"); //insert() should be able to avoid adding duplicates

   cout << "printing s:" << endl;
   s.print();


   if(s.isFinite())
     cout << "s is fininte" << endl;
   if(!s.isFinite())
     cout << "s is cofinite" << endl;
   cout << endl;


   //TEST that isMember() works for a finite set
   if(s.isMember("s1"))
     cout << "s1 is a member" << endl;
   else
     cout << "s1 is NOT a member" << endl;
   
   if(s.isMember("s3"))
     cout << "s3 is a member" << endl;
   else
     cout << "s3 is NOT a member" << endl;

   if(s.isMember("s4"))
     cout << "s4 is a member" << endl;
   else
     cout << "s4 is NOT a member" << endl;

   if(s.isMember("s5"))
     cout << "s5 is a member" << endl;
   else
     cout << "s5 is NOT a member" << endl;

   
   cout << "\n::::::::The complement of s:::::::::" << endl;
   SoS t = s.makeComplement();
   cout << "Array t:" << endl;
   t.print();
   if(t.isFinite())
     cout << "t is finite" << endl;
   else
     cout << "t is cofinite" << endl;
   cout << endl;

   //Test that isMember() works for a cofinite set:
   if(t.isMember("s3"))
     cout << "s3 is a member" << endl;
   else
     cout << "s3 is NOT a member" << endl;

   if(t.isMember("s4"))
     cout << "s4 is a member" << endl;
   else
     cout << "s4 is NOT a member" << endl;

   if(t.isMember("s5"))
     cout << "s5 is a member" << endl;
   else
     cout << "s5 is NOT a member" << endl;
   cout << endl;

   cout << "inserting s5 to t" << endl;
   t.insert("s5");
   cout << "inserting s2 to t" << endl;
   t.insert("s2");
   cout << endl;

   cout << "Array t:" << endl;
   t.print();


   //test of makeUnion:
   SoS k;
   k.insert("k1");
   k.insert("k2");
   k.insert("k3");
   k.insert("k4");
   k.insert("s1");
   cout << "printing k\n";
   k.print();
   cout << endl;

   cout << "printing s\n";
   s.insert("k4");
   s.print();
   cout << endl;

   testMakeUnion(s, k);
   
   SoS p = s.makeComplement();
   testMakeUnion(k, p); //case 2 test
   testMakeUnion(p,k); //case 3 test

   SoS f = k.makeComplement();
   testMakeUnion(f,p);
   testMakeUnion(p,f); //should be the same
   return 0;  
   }

void testMakeUnion(SoS &A, SoS &B)
{
  cout << "::::::::The union of A and B::::::::" << endl;
  cout << "printing A" << endl;
  A.print();
  cout << endl;

  cout << "printing B" << endl;
  B.print();
  cout << endl;

  SoS A_B_union;
  A_B_union = A.makeUnion(B);
  cout << "printing the union of A and B:\n";
  A_B_union.print();


}
