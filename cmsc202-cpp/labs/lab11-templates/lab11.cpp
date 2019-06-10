#include<iostream>
#include "Pair.h"

using namespace std;

int main() {
  /*
  Pair<int> pair1(1, 2);
  Pair<int> pair2(1, 2);
  Pair<int> pair3(2, 1);
  Pair<int> pair4(3, 3);
  */

  Pair<string> pair1("yes", "no");
  Pair<string> pair2("yes", "no");
  Pair<string> pair3("no", "yes");
  Pair<string> pair4("maybe", "not");

  cout << "pair1.first() returned " << pair1.first() << endl;
  cout << "pair1.second() returned " << pair1.second() << endl;

  if (pair1 == pair2) {
    cout << "pair1 == pair2 -- correct\n";
  } else {
    cout << "pair1 != pair2 -- ERROR!\n";
  }

  if (pair1 == pair3) {
    cout << "pair1 == pair3 -- correct\n";
  } else {
    cout << "pair1 != pair3 -- ERROR!\n";
  }

  if (pair1 == pair4) {
    cout << "pair1 == pair4 -- ERROR!\n";
  } else {
    cout << "pair1 != pair4 -- correct\n";
  }
  
  return 0;
}

