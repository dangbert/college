#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <assert.h>
#include <unistd.h>

#include "PinHit.h"
#include "Heap.h"
#include "MinHeap.h"
#include "MaxHeap.h"
using std::cout;
using std::endl;

// global variable for type of heap
std::string heapType = "--max";

//forward declare so I can define it below main
void printGreeting();

std::vector<PinHit> ReadPins(char* fileName, int* totalHits);

template<class T, int m_size>
Heap<T, m_size>* BuildHeap(std::vector<T> PinHits, int slots);

// provided
template<class T, int m_size>
int Hack(Heap<T, m_size>* heap, std::vector<T> PinHits, int totalHits);


int main(int argc, char* argv[]) {
    printGreeting();

    heapType = std::string(argv[2]);
    // ensure the heap type is valid
    assert(heapType == "--min" || heapType == "--max");

    int totalHits = 0;
    // create Vector on PinHit objects and update totalHits
    std::vector<PinHit> pins = ReadPins(argv[1], &totalHits);

    // must call BuildHeap like this because it's a standalone templated function
    // heap must be able to hold 10000 items
    Heap<PinHit, 10000>* heap = BuildHeap<PinHit, 10000>(pins, 10000);
    //heap->LevelPrint(); // print out the Heap

    cout << "-----------------------------" << endl;
    cout << "Using file: " << argv[1] << endl;
    if (heapType == "--min")
        cout << "Using: min heap\n" << endl;
    else
        cout << "Using: max heap\n" << endl;

    int numHacked = Hack(heap, pins, totalHits);
    cout << endl << numHacked << " accounts were hacked." << endl;
    cout << "-----------------------------\n" << endl;

    delete heap;
    return EXIT_SUCCESS;
}

// prints your name and section
void printGreeting() {
    std::cout << "Daniel Engbert, Section 01" << std::endl;
}

/*
 * Read the pin data dump into a vector of Pin Hits. Total hits is an out variable that you will
 * set in the method, tallying the total number of lines (pins) in the file. While “totalHits” is
 * originally passed in as zero, this function will return the final count of totalHits that the
 * vector contains.
 *
 * the vector will have 10000 elements, each index in the vector will store a
 * PinHit where the key = its index, or -1 (if that pin is never found)
 */
std::vector<PinHit> ReadPins(char* fileName, int* totalHits) {
    std::vector<PinHit> pins;
    pins.resize(10000); // set size to 10000, constructs a PinHit at each index

    std::ifstream ifile(fileName); // input file stream
    std::string line;

    while (std::getline(ifile, line)) {
        int pin = atoi(line.c_str()); // convert line to int (0's in front will be lost)

        if (pins[pin].GetKey() == -1) { // this is the first time the pin has been read
            pins[pin].SetKey(pin); // index pin stores a PinHit with pin as the key
        }

        pins[pin].IncrementHits(); // increment number of hits on this object
        *totalHits = *totalHits + 1; // update the total number of hits
    }
    ifile.close();

    return pins;
}

/*
 * This templated function takes the vector full of PinHits and the total size
 * of the heap. This also uses the type of heap (--min or --max) from the
 * command line.
 * note: slots == m_size
 */
template<class T, int m_size>
Heap<T, m_size>* BuildHeap(std::vector<T> PinHits, int slots) {
    Heap<T, m_size>* heap;
    if (heapType == "--min") {
        heap = new MinHeap<T, m_size>();
    }
    else {
        heap = new MaxHeap<T, m_size>();
    }

    // add objects in vector to heap
    for (uint i=0; i<PinHits.size(); i++) {
        if (PinHits.at(i).GetKey() != -1) { // make sure PinHit is valid
            heap->Insert(PinHits.at(i));
        }
    }

    return heap;
}

/*
 * provided function. It will take in a min or max heap and the vector of pins
 * that you read from the file. It will then randomly pick a pin from that vector and use your
 * heap to try to guess what the pin is. You have 1008 attempts (see above). Upon
 * success, a count of successful ‘hacks’ is returned. Try calling this function and
 * comparing the results of using a min heap vs a max heap.
 */
template<class T, int m_size>
int Hack(Heap<T, m_size>* heap, std::vector<T> PinHits, int totalHits) {

    int UPPER_BOUND = totalHits; // used for failsafes

    /*
    * His fictional bank with fictional cards and
    * "normal pins" will allow you to incorrectly attempt
    * to access his card 3 times per hour. You have from
    * the start of the assignment to the end to hack as
    * many accounts as possible (14 days * 24 hr/day * 3
    * attempts/hr = 1008 attempts per card)
    */
    int MAX_ATTEMPTS = 1008;

    int hackAttemptsLeft = MAX_ATTEMPTS;
    int successfulHacks = 0;

    srand (time(NULL)); // seeds our random generator with the current time
    int randomNumHits = rand() % totalHits; // generates a random hit between 0 and total hits
    //randomNumHits = 0; // change this if you want to hardcode in your hits for a specific pin
    int curHits = 0; // our variable for crawling
    int randomIndex = -1; // the index where our new random pinhit lives
    int newHits = 0; // number of hits in new "random" pin
    int failSafe = 0; // always good to have one of these in what could be an infinite loop

    /*
    * Linearly searches through PinHits until
    * we've accrued enough hits to reach our random hits
    * which will then determine the next pin to try to hack.
    * We use newHits to make sure we don't pick a random pin
    * that's not in our heap.
    */
    PinHit curPinHit;
    while (newHits == 0 && failSafe < UPPER_BOUND) {

        for (unsigned int i = 0; i < PinHits.size() && curHits <= randomNumHits; i++) {
            curHits += PinHits[i].GetValue();
            randomIndex = i;
        }
        // should have found the random pin by now
        curPinHit = PinHits[randomIndex];
        newHits = curPinHit.GetValue(); // make sure it's actually in our heap
        //std::cout << "Random Pin: [" << curPinHit.GetKey() << ", " << curPinHit.GetValue() << "] found!" << std::endl;
        failSafe++;
    }

    // let's make a copy of the heap so we don't modify the original.
    Heap<T, m_size>* heapCopy;

    if (heapType == "--min") {
        heapCopy = new MinHeap<T, m_size>(*heap);
    } else if (heapType == "--max") {
        heapCopy = new MaxHeap<T, m_size>(*heap);
    } else {
        //heapCopy = new Heap<T, m_size>(*heap);
        heapCopy = new MaxHeap<T, m_size>(*heap); // I changed this line because my Heap class is abstract
    }

    failSafe = 0;

    PinHit defaultPin;

    // exit once hacking 10000 accounts, either a hacking prodigy or something is wrong
    int MAX_NUM_HACKS = 10000;

    while (0 < hackAttemptsLeft && successfulHacks < MAX_NUM_HACKS) {

        // grabs our root
        PinHit topOfHeap = heapCopy->Remove();
        if (topOfHeap == defaultPin)
            break;

        /*
        std::cout << topOfHeap.GetKey() << ", " << topOfHeap.GetValue() << " our last attempt" << std::endl;

        if (hackAttemptsLeft == 1)
        std::cout << topOfHeap.GetKey() << ", " << topOfHeap.GetValue() << " our last attempt" << std::endl;
        */

        hackAttemptsLeft--;

        // account was hacked!
        if (topOfHeap == curPinHit) {
            //std::cout << "We got them!" << std::endl;
            std::cout << "Account with Pin " << curPinHit.GetKey() << " hacked!" << std::endl;
            successfulHacks++;

            // reset our heap
            delete heapCopy;
            if (heapType == "--min") {
                heapCopy = new MinHeap<T, m_size>(*heap);
            } else if (heapType == "--max") {
                heapCopy = new MaxHeap<T, m_size>(*heap);
            } else {
                //heapCopy = new Heap<T, m_size>(*heap);
                heapCopy = new MaxHeap<T, m_size>(*heap); // I changed this line because my Heap class is abstract
            }

            // reset
            hackAttemptsLeft = MAX_ATTEMPTS;
            randomNumHits = rand() % totalHits;
            curHits = 0;
            randomIndex = -1;
            newHits = 0;
            failSafe = 0;
            while (newHits == 0 && failSafe < UPPER_BOUND) {

                for (unsigned int i = 0; i < PinHits.size() && curHits <= randomNumHits; i++) {
                    curHits += PinHits[i].GetValue();
                    randomIndex = i;
                }
                // should have found the random pin by now
                curPinHit = PinHits[randomIndex];
                newHits = curPinHit.GetValue(); // make sure it's actually in our heap
                //std::cout << "Random Pin: [" << curPinHit.GetKey() << ", " << curPinHit.GetValue() << "] found!" << std::endl;
                failSafe++;
            }

            //std::cout << "Valid Random Pin: " << curPinHit.GetKey() << " found!" << std::endl;
        }
    }

    if (heapCopy != NULL) {
        delete heapCopy;
    }

    return successfulHacks;

}
