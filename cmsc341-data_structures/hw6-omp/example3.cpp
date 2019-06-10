// File: example3.cpp
//
#include <stdio.h>
#include <omp.h>

int main() {

   int reps = 10000 ;
   long int A[reps] ;
   long int B[reps] ;
   long int sumA = 0 ;
   long int sumB = 0 ;

   for (long int i=0; i < reps ; i++) {
      A[i] = i ;
      B[i] = 2*i ;
   }

   #pragma omp parallel shared(A,B,sumA,sumB,reps)
   {
      #pragma omp single
      {  // only one thread has to do this
         omp_set_num_threads(4);
         printf("Number of threads = %d\n", omp_get_num_threads() ) ;
      }

      #pragma omp for schedule(static,10) reduction(+:sumA) reduction(+:sumB)

      for (long int i=0; i < reps; i++) {
         sumA += A[i] ;
         sumB += B[i] ;
      }

   } // end of parallel region

   printf("sumA = %ld\n", sumA) ;
   printf("sumB = %ld\n", sumB) ;


   return 0 ;
}
