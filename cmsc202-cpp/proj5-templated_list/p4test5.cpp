// File: p4test5.cpp
//
// UMBC CMSC 341 Fall 2017 Project 4
//
// Testing MedianHeap with City class and sanityCheck
//
// Run program with data redirected as standard input:
//   ./a.out < data.txt
//


#include <iostream>
#include <cstdlib>
using namespace std ;

#include "MedianHeap.h"
#include "City.h"


template <typename T>
void sanityCheck(MedianHeap<T>& H, bool (*lt) (const T&, const T&), bool (*gt) (const T&, const T&), City *cities, int totalCnt, int status_bit) {
   T minKey, maxKey, leftKey, rightKey ;
   int minPos, maxPos, pos ;
   
   bool passed=true ;

   cout << "=====================================================\n" ;
   cout << "sanityCheck called ...\n" ;

   int maxHSize = H.maxHeapSize() ;
   int minHSize = H.minHeapSize() ;
   int mHSize = H.size() ;

   if ( mHSize != totalCnt ) {
       cout << "MedianHeap size is incorrect: expected " << totalCnt <<
	   ", got " << mHSize << endl;
       passed = false;
   }
   if ( mHSize == maxHSize + minHSize ) {
      cout << "max and min heap sizes are consistent MedianHeap::size()\n" ;
   } else {
      cout << "max and min sizes are NOT consistent with MedianHeap::size()\n" ;
       passed = false;
   }

   if (maxHSize < minHSize + 2 && minHSize < maxHSize + 2) {
      cout << "max and min heap sizes are within 1 of each other\n" ;
   } else {
      cout << "max heap or min heap too big\n" 
           << "   max heap size = " << maxHSize << endl 
           << "   min heap size = " << minHSize << endl ;
      passed = false;
   }

   // Check that everything in max heap < min heap
   //
   if (maxHSize > 0 && minHSize > 0) {
      T maxHroot = H.locateInMaxHeap(1) ;
      T minHroot = H.locateInMinHeap(1) ;
      if (gt(maxHroot,minHroot) ) {
        passed = false ;
	cout << "*** Root of max heap > root of min heap\n" ;
      }
   }


   cout << "Sanity Check maxHeap...\n" ;

   for (int i=1; i <= maxHSize ; i++) {
      maxKey = H.locateInMaxHeap(i) ; 

      if (!(cities[maxKey.m_index].m_status & status_bit)) {
	  cout << "Found deleted city (" << maxKey << ")" << endl;
	  passed = false;
      }
      if (2*i <= maxHSize) {  // has left child
         leftKey = H.locateInMaxHeap(2*i) ;
	 if ( gt(leftKey,maxKey) ) {  // left child bigger than root 
	    passed = false ;
	    cout << "Bad heap condition at i = " << i << ": "
		 << "root key = " << maxKey << ", " 
		 << "left key = " << leftKey 
		 << endl ;
	 }
      }  

      if (2*i+1 <= maxHSize) {  // has right child
         rightKey = H.locateInMaxHeap(2*i+1) ;
	 if ( gt(rightKey,maxKey) ) {  // right child bigger than root 
	    passed = false ;
	    cout << "Bad heap condition at i = " << i << ": "
		 << "root key = " << maxKey << ", " 
		 << "right key = " << rightKey 
		 << endl ;
	 }
      }  
   }  // end of for (...)


   cout << "Sanity Check minHeap...\n" ;

   for (int i=1; i <= minHSize ; i++) {
      minKey = H.locateInMinHeap(i) ; 

      if (!(cities[minKey.m_index].m_status & status_bit)) {
	  cout << "Found deleted city (" << minKey << ")" << endl;
	  passed = false;
      }
      if (2*i <= minHSize) {  // has right child
         leftKey = H.locateInMinHeap(2*i) ; // left child smaller than root 
	 if ( lt(leftKey,minKey) ) {  
	    passed = false ;
	    cout << "Bad heap condition at i = " << i << ": "
		 << "root key = " << minKey << ", " 
		 << "left key = " << leftKey 
		 << endl ;
	 }
      }  

      if (2*i+1 <= minHSize) {  // has right child
         rightKey = H.locateInMinHeap(2*i+1) ;
	 if ( lt(rightKey,minKey) ) {  // right child smaller than root 
	    passed = false ;
	    cout << "Bad heap condition at i = " << i << ": "
		 << "root key = " << minKey << ", " 
		 << "right key = " << rightKey 
		 << endl ;
	 }
      }  

   }  // end of for (...)



   if (passed) {
      cout << "Passed sanityCheck().\n" ;
   } else {
      cout << "*** Failed sanityCheck().\n" ;
   }

   cout << "=====================================================\n" ;
}


int main() {
   string cityName, state ;
   float latitude, longitude ;
   int population, elevation ;
   int numItems ;
   
   // first line of data file has number of items
   //
   cin >> numItems ;

   // Some place to hold all of the cities
   //
   City *cities = new City[numItems] ; 
   int cityCnt = numItems;


   // Get City data from a file (stdin)
   //
   cout << "Read in some cities: ...\n" ;

   for (int i=0 ; i < numItems ; i++) {
     
      cin >> cities[i] ;

      if ( cin.eof() ) {
         cerr << "Someone lied about the length of the file!\n" ;
	 exit(1) ;
      }

      cities[i].m_index = i;
      cities[i].m_status = 3;
      // cout << cities[i] << endl ;
   }


   // Declare MedianHeaps for population and elevation
   //
   MedianHeap<City> PopH (City::smallerPopulation, City::biggerPopulation, numItems) ;
   MedianHeap<City> ElevH (City::smallerElevation, City::biggerElevation, numItems) ;


   // Add items to the MedianHeaps
   //
   for (int i=0 ; i < numItems ; i++) {
      PopH.insert(cities[i]) ;
      ElevH.insert(cities[i]) ;
   }

   // Remove some items from the MedianHeaps
   //
   City tempCity ;

   for (int i=0 ; i < numItems ; i+=7) {
      tempCity = cities[i] ; 
      if (!PopH.deleteItem(tempCity,City::sameCity)) {
	  cout << "PopH deleteItem(" << tempCity << ") FAILED!\n";
      } else {
	  cities[i].m_status -= 1;
      }
      if (!ElevH.deleteItem(tempCity,City::sameCity)) {
	  cout << "ElevH deleteItem(" << tempCity << ") FAILED!\n";
      } else {
	  cities[i].m_status -= 2;
      }
      --cityCnt;
   }

   cout << "\nChecking Population MedianHeap...\n" ;
   cout << "   min    = " << PopH.getMin() << endl ;
   cout << "   median = " << PopH.getMedian() << endl ;
   cout << "   max    = " << PopH.getMax() << endl ;
   sanityCheck(PopH,City::smallerPopulation,City::biggerPopulation, cities,
	       cityCnt, 1) ;
   
   cout << "\nChecking Elevation MedianHeap...\n" ;
   cout << "   min    = " << ElevH.getMin() << endl ;
   cout << "   median = " << ElevH.getMedian() << endl ;
   cout << "   max    = " << ElevH.getMax() << endl ;
   sanityCheck(ElevH,City::smallerElevation,City::biggerElevation, cities,
	       cityCnt, 2) ;

   delete [] cities ;

   return 0 ;
}
