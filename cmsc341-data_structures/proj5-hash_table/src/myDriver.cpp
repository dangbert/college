// File: mydriver.cpp
// based on mediumtest.cpp

#include <iostream>
#include <time.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "words.h"
#include "Pinball.h"

using namespace std;

// function prototypes
int myRand(int seed=0);
void test(int size, int reps);
void testFully(int size);

int main() {
	// set the seed based on the current time
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    printf("Random seed set to: %ld\n\n", ms);
    myRand(ms);
    // myRand(8781035); // Uncomment to use same random seed each time


    testFully(5003);
    testFully(10037);
    testFully(20101);

    return 0;

    test(5003,4000) ;   // >5,000 slots, ~80% full
    printf("\n\n") ;

    test(10037,8000) ;  // >10,000 slots, ~80% full
    printf("\n\n") ;

    test(20101,16000) ;  // >20,000 slots, ~80% full
    printf("\n\n") ;

}

// test using the given size at 50%, 60%, 70%, 80%, and 90% full
// test 10 times at each size
void testFully(int size) {
    int reps;

    for (int i=50; i<=90; i+=10) {
        cout << "-------------------------------------------------------" << endl;
        cout << "-------------Testing size " << size << " at " << i << "% full-------------" << endl;
        cout << "-------------------------------------------------------" << endl;
        reps = size * i / 100;

        cout << "reps = " << reps << endl;
        for (int i=1; i<=10; i++) {
            test(size, reps);
        }
        cout << "\n\n\n" << endl;
    }

}

// Get amount of user time since the program began in milliseconds.
double getmsecs() {
   struct rusage use;
   struct timeval utime;

   getrusage(RUSAGE_SELF, &use);
   utime = use.ru_utime;
   return ((double) utime.tv_sec)*1000.0 +  ((double) utime.tv_usec)/1000.0;
}


// This version uses lrand48().
// Set the random seed by passing a non-zero parameter.
//
int myRand(int seed) {
   if (seed) srand48(seed);
   return lrand48();
}


/*
 * tests the Pinball class and outputs the resulting stats
 * @param size: the size of the hash table
 * @param reps: number of words to insert into the hash table
 */
void test(int size, int reps) {
    double startTime, stopTime;

    Pinball PH = Pinball(size);
    int saveIndices[reps];  // array to remember which words were picked
    for (int i=0; i < reps; i++) {
        saveIndices [i] = -1; // initialize all values to -1
    }

    startTime = getmsecs();

    int index;
    int slot;

    ///cout << "Inserting some words..." << endl;
    bool exceptionThrown = false; // true if a PinballHashFull error is thrown
    // Pick some words from global words[] array to insert in the hash table. 
    for (int i=0; i < reps; i++) {
        // find a word that isn't already in the hash table
        do {
            index = myRand() % numWords;
            slot = PH.find(words[index]);
        }
        while(slot > -1); // while word already exists in the hash table

        saveIndices[i] = index;    // save indices of the picked words

        try {
            PH.insert(words[index]); // insert the word into the hash table

        } catch (PinballHashFull &e) {
            cout << e.what() << endl;
            exceptionThrown = true;
            //cout << "   iteration = " << i << endl;
            //cout << "   words[" << index << "] = " << words[index] << endl;
            break;
        } catch (...) {
            cout << "Unknown error\n";
        }
    }
    ///cout << "...complete!\n\n";

    stopTime = getmsecs();
    printf("Elapsed time = %.3lf milliseconds\n", stopTime - startTime);

    PH.printStats();
    cout << "\n\n";
    return;

    // (the code below is for debugging)

    // check that all the inserted words are really there (for debugging)
    cout << "verifying that all inserted words are really there..." << endl;
    bool error = false; // used to indicate if a test passed or not
    int notFound = 0; // number of words not found
    for (int i=0; i < reps; i++) {
        index = saveIndices[i];
        if (index < 0) continue;  // maybe exception thrown?

        slot = PH.find(words[index]);

        if (slot == -1) {
            notFound++;
            cout << "   Inserted word not found: " << words[index] << endl;
        }
        else if (strcmp(PH.at(slot),words[index]) != 0) {
            error = true;
            cout << "   ERROR: Found word not equal!\n";    
        }
    }

    if (error)
        cout << "...test failed.\n\n";
    else if (exceptionThrown && notFound <= 1)
        cout << "...test passed! (1 word not found due to ejection limit)\n\n";
    else
        cout << "...test passed!\n\n";


    // Pick some words to be removed from the hash table
    bool passed = true;
    char *str;
    cout << "Removing some words...\n";
    for (int j = 0; j < 20; j++) {
        index = saveIndices[myRand() % reps];
        if (index < 0) continue;  // maybe exception thrown?

        str = PH.remove(words[index]);  // recover words that were picked

        if (str != NULL && strcmp(str,words[index]) != 0) {
            passed = false;
            cout << "Wrong word removed!" << endl
                << "   str = " << str << endl 
                << "   words[" << index << "] = " 
                << words[index] << endl;
        }
        free(str);
    }
    if (passed)
        cout << "...words were successfully removed!\n" << endl;
    else 
        cout << "...test failed.\n" << endl;

    PH.printStats();
}
