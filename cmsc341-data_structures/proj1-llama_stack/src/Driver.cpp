#include <iostream>
#include <string>
#include <stdlib.h>
#include "Llama.h"
#include "OvCoInt.h"

using namespace std;

// my driver
int main() {

    // create Llama s
    const int num = 6;
    Llama<char, num> s;
    for (int i=122; i>=97; i--) {
        s.push(i);
    }

    cerr << endl << endl << "____________________Testing s.dump()____________________" << endl;
    cerr << "Original s:" << endl;
    s.dump();

    cerr << endl << endl << "____________________Testing peek() on s____________________" << endl;
    for (int i=0; i<s.size(); i++) {
        cerr << i << ": " << s.peek(i) << endl; }


    cerr << endl << endl << "____________________Testing s.swap()____________________" << endl;
    s.swap();
    cerr << "New s:" << endl;
    s.dump();


    cerr << endl << endl << "____________________Testing s.rot()____________________" << endl;
    s.rot();
    cerr << "New s:" << endl;
    s.dump();


    cerr << endl << endl << "____________________Testing s.dup()____________________" << endl;
    s.dup();
    cerr << "New s:" << endl;
    s.dump();
    cerr << "_____________________________________________________" << endl;

    cerr << "Popped s until it had an extra node. New s:" << endl;
    s.pop();
    s.pop();
    s.pop();
    s.pop();
    s.dump();

    cerr << endl << endl << "____________________Testing copy constructor____________________" << endl;
    cerr << "COPIED: VERSION of s" << endl;
    Llama<char, num> c(s);
    c.dump();


    cerr << endl << endl << "____________________Testing assignment operator____________________" << endl;
    Llama<char, num> t;
    for (int i=122; i>=100; i--) {
        t.push(i);
    }
    for (int i=65; i<=65+26; i++) {
        t.push(i);
    }
    cerr << "t INITIAL:" << endl;
    t.dump();
    cerr << "t after assigning t=s:" << endl;
    t = s;
    t.dump();

    cerr << "ASSIGNED COPY of s:" << endl;
    Llama<char, num> a;
    a = s;
    a.dump();

    // duplicate
    cerr << "assigning a to s" << endl;
    a = s;
    a.dump();

    cerr << "assigning s to s" << endl;
    s = s;
    s.dump();

    return 0;
}
