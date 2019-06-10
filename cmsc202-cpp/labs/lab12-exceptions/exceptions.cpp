#include <vector>
#include <iostream>
#include <sstream>

using namespace std;
class StackEmpty {
public:
  StackEmpty(string s) {
    msg = s;
  }
  string GetMessage() {
    return msg;
  }

private:
  string msg;
};


class StackFull {
public:
  StackFull(string s, int n) {
    msg = s;
    num = n;
  }

  string GetMessage() {
     return msg;
  }

  int GetValue() {
    return num;
  }

private:
  string msg;
  int num;
};

class InvalidSize {
public:
  InvalidSize(string s) {
    msg = s;
  }

  string GetMessage() {
    return msg;
  }

private:
  string msg;

};



class IntStack
{
public:
  // MaxSize should be an unsigned int, but for the sake of example...
  IntStack(int MaxSize)
  {
    if(MaxSize < 0)
      {
	//cerr << "Cannot create a negative size stack" << endl;	
	///exit(1);
      throw InvalidSize("Cannot create a negative size stack");
      }
    else {	
      data.resize(MaxSize);
      cur_index = 0;
    }
  }

  void push(int new_data)
  {
    
    if(cur_index == data.size())
      {
	///cerr << "Push to full stack" << endl;
	///exit(1);
	throw StackFull("Push to full stack", cur_index);
      }
    else
      {
	data.at(cur_index) = new_data;
	cur_index++;
      }

  }

  int pop()
  {
    if(cur_index == 0)
      {
	//cerr << "Pop from empty stack" << endl;
	//exit(1);
	throw StackEmpty("Pop from empty stack");
      }
    else
      {
	// pop off the int and return it
	cur_index--;
	return data.at(cur_index);
      }
  }

private:
  vector<int> data;
  unsigned int cur_index; //stores the index that will be pushed onto next
};



int main()
{
  // Testing Constructor
  try {
    IntStack c_test(-10);
    c_test.push(3);
    c_test.push(4);
    c_test.pop();
    c_test.pop();
  }
  catch(InvalidSize ex) {
    cerr << ex.GetMessage() << endl;
  }
	
  // Testing push
  IntStack push_test(5);
	
  try {
    for(unsigned int i = 0; i < 7; i++) {
      push_test.push(i);
    }
  }
  catch(StackFull ex) {
    cerr << ex.GetMessage() << ", value = " << ex.GetValue() << endl;
  }
  
	
	
  // Testing pop
  IntStack pop_test(2);
	
  pop_test.push(1);
  pop_test.push(2);
  pop_test.pop();
  pop_test.pop();
  try {
    pop_test.pop();
  }
  catch(StackEmpty ex) {
    cerr << ex.GetMessage() << endl;
  }
	
  cout << "Completed!" << endl;
}
