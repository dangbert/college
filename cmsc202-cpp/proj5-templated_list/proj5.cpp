#include <iostream>
#include "node.h"
#include "list.h"

using namespace std;

int main() {
  LinkedList<int> L;
  cout << endl << ":::::::::::Testing addFront() and size():::::::::::" << endl;
  cout << "initialized size()=" << L.size() << endl;
  L.addFront(2);
  L.addFront(4);
  L.addFront(-1);
  L.addFront(17);
  L.addFront(38);
  L.addFront(-9);

  cout << "Printing with [] operator" << endl;
  cout << "size()=" << L.size() << endl;
  for(int i=0; i<L.size(); i++)
    cout << "L[" << i << "] = " << L[i] << endl;

  cout << endl << "Printing with print() function" << endl;
  L.print();


  cout << endl << ":::::::::::Testing deleteFront():::::::::" << endl;
 
  while(L.size() != 0) {
    cout << "removed " << L.deleteFront();
    cout << ". size()=" << L.size() << "  printing new list:" << endl;
    L.print();
    cout << endl;
  }

  cout << endl << ":::::::::::Testing deleteFront() on an empty list:::::::::" << endl;
  LinkedList<int> l2;
  cout << "list l2:" << endl;
  l2.print();
  try {
    l2.deleteFront();
  }
  catch(range_error r) {
    cerr << r.what() << endl;
  }


  return 0;
}
