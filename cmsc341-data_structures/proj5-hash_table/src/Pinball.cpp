/*
 * Daniel Engbert
 * CMSC 341 Section 01, Fall 2016
 *
 * This class implements a hash table using a variation of Cuckoo hashing.

 * my assigned values:
 * degree: 5
 * ejection limit: 20
 * define auillary slots by generating random offsets (Option 3)
 */

#include "Pinball.h"
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdexcept>
using namespace std;


/*
 * Constructor
 * @param n: the size of the hash table (default is the prime number 1019)
 */
Pinball::Pinball(int n) {
    H = new char*[n](); // the hash table, the () sets all indices to NULL
    m_primarys = new bool[n]; // indicates when an index in H is used as a primarySlot
    m_primaryHits = new int[n];
    for (int i=0; i<n; i++) {
        m_primarys[i] = false;
        m_primaryHits[i] = 0;
    }

    m_capacity = n;     // number of slots in H
    m_size = 0;         // number of items stored in H
    m_degree = 5;       // my assigned value
    m_ejectLimit = 20;  // my assigned value
    numEjections = 0;
    maxEjections = 0;
    cumlEjections = 0;
    stillEjecting = false;
    assert(m_degree <= m_capacity); // must be possible to have all unique offsets

    // create m_offsets (unique values)
    m_offset = new unsigned int[m_degree]; // one offset per slot
    m_offset[0] = 0; // represents the primary slot

    for (int i=1; i<m_degree; i++) { // iterate through rest of m_offset
        bool unique = false;

        while (!unique) {
            m_offset[i] = rand() % m_capacity; // generate an offset value
            // compare against all prior offsets to make sure its unique
            for (int k=0; k<i; k++) {
                if (m_offset[i] == m_offset[k]) // if not unique
                    break;
                if (k == i-1) // its unique
                    unique = true;
            }
        }
    }
}

/*
 * Destructor
 */
Pinball::~Pinball() {
    delete [] m_primarys;
    delete [] m_primaryHits;
    delete [] m_offset;

    for (int i=0; i<m_capacity; i++) {
        free(H[i]); // deallocate memory that was initialized with malloc() by strdup()
    }
    delete [] H;
}

/*
 * Inserts a copy of the C string str into the hash table.
 * @param str: pointer to a string to copy and insert into the hash table.
 *
 * Throws a PinballHashfull excpetion when the hash table is full
 * or the ejection limit is exceeded.
 * Strings that already exist in the hash table won't be inserted again.
 */
void Pinball::insert(const char *str) {
    if (find(str) != -1)
        return; // don't insert a string that's already in the table

    char* value = strdup(str); // copy of str (free memory with free() later)
    unsigned int primarySlot = hashCode(str) % m_capacity;

    // look for an open slot
    // (i=0 corresponds to the primarySlot)
    for (int i=0; i<m_degree; i++) { // loop through all the offsets
        int slot = (primarySlot + m_offset[i]) % m_capacity;
        if (H[slot] == NULL) {  // if this slot is free

            if (stillEjecting) {
                if (numEjections > maxEjections) // update maxEjections if needed
                    maxEjections = numEjections;
                stillEjecting = false; // the ejection process has now stopped
            }

            // insert into this slot
            H[slot] = value;
            m_size++;
            numEjections = 0; // reset the number of ejections

            // update statistics variables:
            // keep track of how many things hash to each primarySlot
            m_primaryHits[primarySlot]++;

            // keep track of which primary slots are used
            if (i == 0) // if this is the primarySlot for str
                m_primarys[slot] = true;
            else
                m_primarys[slot] = false; // makes sure this is false
            return;
        }
    }

    // all possible slots are full
    // check that the ejectionLimit hasn't been reached
    if (numEjections == m_ejectLimit) {
        // update numEjections since it didn't get a chance to update
        maxEjections = numEjections;
        // give up on inserting this string, declare the hash table full
        free(value); // free the string that was going to be inserted
        throw PinballHashFull("*** Exception: Ejection limit exceeded!");
    }


    // randomly chose one auxillary slot to free up by ejecting its value
    stillEjecting = true; // note that the ejection process is occurring
    int index = 1 + rand() % (m_degree-1); // random int between 1 and (m_degree-1)
    int auxSlot = (primarySlot + m_offset[index]) % m_capacity; // index of the auxillary slot

    // eject from auxSlot and then use the spot (m_size stays the same)
    char* ejected = H[auxSlot];
    H[auxSlot] = value;
    numEjections++; // increment the number of ejections
    cumlEjections++;

    // keep track of how many things hash to each primarySlot
    m_primaryHits[primarySlot]++;
    unsigned int ejected_primarySlot = hashCode(ejected) % m_capacity;
    m_primaryHits[ejected_primarySlot]--; // incremented again later by insert()

    m_primarys[auxSlot] = false; // this isn't a primary slot

    try {
        insert(ejected);
    } catch (PinballHashFull &e) {
        // catch the exception so that memory can be freed
        free(ejected);
        throw e; // now rethrow the exception
    }
    free(ejected); // free ejected because the call to insert() stored a copy
}

/*
 * Looks for a string in the hash table and returns its index in H.
 * @param str: the c string to search for.
 *
 * returns -1 if the string doesn't exist in the hash table.
 * The index returned is only valid until the next call to insert() or remove().
 */
int Pinball::find(const char *str) {
    unsigned int primarySlot = hashCode(str) % m_capacity;

    for (int i=0; i<m_degree; i++) { // look in all possible slots
        int slot = (primarySlot + m_offset[i]) % m_capacity;
        if (H[slot] != NULL) {
            if (strcmp(H[slot], str) == 0) // if str is found here
                return slot;
        }
    }
    return -1;
}

/*
 * Returns a const pointer to the string stored at the given index in H.
 * @param index: the index to retrieve the string from.
 *
 * If the index doesn't exist in H, then an out_of_range error is thrown.
 */
const char* Pinball::at(int index) {
    if (!(0 <= index && index < m_capacity)) {
        throw std::out_of_range("Index out of range in at() function.");
    }
    return H[index]; // could be NULL
}

/*
 * Removes a string from the hash table and returns its pointer.
 * @param str: the string to find and remove from the hash table.
 *
 * Returns NULL if the string is not in the hash table.
 * The code that calls remove() must free the pointer that is returned.
 */
char* Pinball::remove(const char *str) {
    int index = find(str);
    if (index == -1)
        return NULL;

    char* removed = H[index]; // pointer to the string being removed
    H[index] = NULL; // remove the target string from the Hash table
    m_primarys[index] = false;
    m_size--;

    unsigned int removed_primarySlot = hashCode(removed) % m_capacity;
    m_primaryHits[removed_primarySlot]--;

    return removed;
}

/*
 * Print out some statistics about the hash table. (See sample output.) This
 * requires cooperation from insert() and remove() and additional data members
 * in the class. 
 * (See additional clarifications in project description)
 */
void Pinball::printStats() {
    int numPrimary = 0; // number of slots currently filled as primary slots in the table
    int maxPrimaryHits = 0;
    for (int i=0; i<m_capacity; i++) {
        if (m_primarys[i])
            numPrimary++;

        if (m_primaryHits[i] > maxPrimaryHits)
            maxPrimaryHits = m_primaryHits[i];
    }

    cout << "Hash statistics report:" << endl;
    cout << "   randomness level = not very random" << endl; // my assigned option
    cout << "   capacity = " << m_capacity << endl;
    cout << "   size = " << m_size << endl;
    cout << "   degree = " << m_degree << endl;
    cout << "   ejection limit = " << m_ejectLimit << endl;
    cout << "   number of primary slots = " << numPrimary << endl;
    cout << "   average hits to primary slots = " << (float) m_size / numPrimary << endl;

    // maximum hits to primary slots is the maximum # of items that hash to the
    // same primary slot considering only the items currently in the hash table
    cout << "   maximum hits to primary slots = " << maxPrimaryHits << endl;
    cout << "   total number of ejections = " << cumlEjections << endl;
    cout << "   maximum number of ejections in a single insertion = " << maxEjections << endl;
}
