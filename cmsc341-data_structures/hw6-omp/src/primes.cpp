#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <iostream>
using std::cout;
using std::endl;

bool isPrime (unsigned int n);

// counts the number of prime numbers up to 10,000,000
int main() {
    long int numPrimes = 0; // number of primes found
    long int reps = 10000000;

    #pragma omp parallel shared(numPrimes,reps)
    {
        #pragma omp single
        {  // only one thread has to do this
            omp_set_num_threads(4);
            printf("Number of threads = %d\n", omp_get_num_threads() );
        }

        // the reduction clause, reduction(+:sum), tells OpenMP to generate
        // code that uses the + operator to add together the value of numPrimes
        // at the end of the for loop so that updates to the shared variable
        // numPrimes aren't lost as it's being updated by different threads.
        #pragma omp for schedule(static,10) reduction(+:numPrimes)

        for (long int i=1; i<=reps; i++) {
            if (isPrime(i)) {
                numPrimes++;
            }
        }

    } // end of parallel region

    printf("primes found = %ld\n", numPrimes);

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
