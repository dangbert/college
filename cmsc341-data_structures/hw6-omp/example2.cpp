// File: example2.cpp
//
#include <stdio.h>
#include <omp.h>

int main() {

   int reps = 10000 ;
   long int A[reps] ;
   long int sum = 0 ;

   for (long int i=0; i < reps ; i++) {
      A[i] = i ;
   }

   #pragma omp parallel shared(A,sum,reps)
   {
      #pragma omp single
      {  // only one thread has to do this
         omp_set_num_threads(4);
         printf("Number of threads = %d\n", omp_get_num_threads() ) ;
      }

      #pragma omp for schedule(static,5) reduction(+:sum)
      for (long int i=0; i < reps; i++) {
         sum += A[i] ;
      }

   } // end of parallel region

   printf("sum = %ld\n", sum) ;


   return 0 ;
}
