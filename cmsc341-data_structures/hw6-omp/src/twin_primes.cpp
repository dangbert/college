#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <iostream>
#include <list>
using std::cout;
using std::endl;

bool isPrime (unsigned int n);

// indicates if twin primes (a pair or primes that differ by 2) exist between
// start and stop and if so, it prints the pairs.
int main() {
    bool twinPrimes = false; // if twin primes were found
    long int start = 1000000;
    long int stop = 2000000;

    std::list<long int> L1; // holds the first prime in each pair
    std::list<long int> L2; // holds the second prime in earch pair

    #pragma omp parallel shared(twinPrimes,start,stop,L1,L2)
    {
        #pragma omp single
        {  // only one thread has to do this
            omp_set_num_threads(4);
            printf("Number of threads = %d\n", omp_get_num_threads() );
        }

        // The reduction clause with the || operator will determine if there
        // are any twin primes in the range.
        #pragma omp for schedule(static,10) reduction(||:twinPrimes)

        for (long int i=start; i<=(stop-2); i++) {
            if (isPrime(i) && isPrime(i+2)) {
                twinPrimes = true;

                // guarantees that when a thread inserts a number in L1, no
                // other thread is trying to modify L2.
                #pragma omp critical
                {
                    L1.push_back(i);
                    L2.push_back(i+2);
                }
            }
        }
    } // end of parallel region

    if (twinPrimes) {
        printf("Yes, there are twin primes between %ld and %ld.\n", start, stop);

        // print out all the twin prime pairs that were found
        int index = 0;
        for (std::list<long int>::const_iterator it = L1.begin(); it != L1.end(); it++) {
            cout << *it << ", ";

            // get the element in list2 at the same position
            std::list<long int>:: const_iterator it2 = L2.begin();
            std::advance(it2, index); // advance iterator to same position (inefficient)
            cout << *it2 << endl;

            index++;
        }

    }
    else {
        printf("No, there are not twin primes between %ld and %ld.\n", start, stop);
    }

    return 0;
}

/*
 * returns true if an integer is prime, returns false otherwise
 */
bool isPrime (unsigned int n) {
    unsigned int m = sqrt(n);

    for (unsigned int k=2; k <= m; k++) {
        if (n % k == 0)
            return false; 
    }

    if (n <= 1) // 0 and 1 aren't primes
        return false;

    return true; 
}
