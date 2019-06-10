// File: game.cpp
//
// Version 2015-11-21: fixed self-encounter bug in playOneTurn
//
// The Game class runs the game.
// The important function is playOneTurn. This 
// function handles removing dead characters,
// selecting random characters for encounters,
// migration and morphing.
//
// *** Do not change any part of this file. ***
//
#include <iostream>
#include <stdlib.h>

using namespace std ;

#include "game.h"
#include "character.h"


Game::Game() {
   m_numRooms = 0 ;
   m_map = NULL ;
   m_numAdjacent = NULL ;
   m_CharactersInRoom = NULL ;
}


Game::Game(unsigned int n, bool *map, long seed) {

   srand48(seed) ;  // set random seed

   unsigned int adjacent ;

   // set up map.
   m_numRooms = n ;
   m_map = new bool *[n] ;
   m_numAdjacent = new unsigned int[n] ;

   for (unsigned int i = 0 ; i < n ; i++) {
      m_map[i] = new bool[n] ;
   }

   for (unsigned int i = 0 ; i < n ; i++) {

      adjacent = 0 ; 

      for (unsigned int j = 0 ; j < n ; j++) {

         m_map[i][j] = map[i*n+j] ;    // copy to 2D array
         if (m_map[i][j]) adjacent++ ; // update neighbor count

      }

      m_numAdjacent[i] = adjacent ;  // store neighbors
   }

   m_CharactersInRoom = new LinkedList[n] ;
   m_CharactersNextRound = new LinkedList[n] ;

}


Game::~Game() {

   // cout << "Deleting CharactersInRoom[]\n" ;
   delete [] m_CharactersInRoom ;

   // cout << "Deleting CharactersNextRound[]\n" ;
   delete [] m_CharactersNextRound ;

   // cout << "Deleting Adjacent[]\n" ;
   delete [] m_numAdjacent ;

   // cout << "Deleting m_map[][]\n" ;
   for (int i=0 ; i < m_numRooms ; i++) {
      delete [] m_map[i] ;
   }
   delete [] m_map ;
}



void Game::playOneTurn() {
   Node *current ;
   unsigned int listSize ;
   unsigned int index ;
   unsigned int randomIndex ;
   Character *thisChar, *otherChar ;


   // need to remove dead characters

   LinkedList tempList ;
   Node *ptr ;

   // for each room and each Character in the room,
   // check if the Character is dead. If so, don't copy
   // over to tempList.

   for (unsigned int r = 0 ; r < m_numRooms ; r++) {
      while ( ( ptr = m_CharactersInRoom[r].popNode() ) != NULL ) {
         if ( ptr->m_cptr->isDead() ) {
            cout << "Removing " << ptr->m_cptr->identify() << " (dead)\n" ;
            delete ptr ;
         } else {
            tempList.addNode(ptr) ;
         }
      }
      m_CharactersInRoom[r].slurp(tempList) ;
   }


   // For each room and each Character in the room,
   // randomly pick a Character in the same room,
   // and have an encounter with the host.

   for (unsigned int r = 0 ; r < m_numRooms ; r++) {

      listSize = m_CharactersInRoom[r].size() ;
      if ( listSize <= 1) continue ;

      index = 0 ;
      current = m_CharactersInRoom[r].m_header ;
      while (current->m_next != NULL) {

         current = current->m_next ;
         thisChar = current->m_cptr ;

         if (thisChar->isDead()) {   // no encounters for the dead
            index++ ;                // bug fix: must increment index
            continue ;  
         }

         // Calculate a random # that skips over the host.
         randomIndex = lrand48() % (listSize - 1) ;
         if (randomIndex >= index) randomIndex++ ;


         // Fetch another Character in the same room
         otherChar = m_CharactersInRoom[r].at(randomIndex)->m_cptr ;

         // *** MOST IMPORTANT PART *** 

         thisChar->encounter(otherChar) ; 

         index++ ;
      }
   }


   // For each room and each Character living in that room,
   // move that Character to a nearby room.
   // 
   unsigned int newRoom, oldRoom ;

   for (unsigned int r = 0 ; r < m_numRooms ; r++) {

      while ( ( ptr = m_CharactersInRoom[r].popNode() ) != NULL ) {

         thisChar = ptr->m_cptr ;

         if ( thisChar->migrates() )  {  // some Characters don't migrate

            newRoom = getRandomAdjacentRoom(r) ;

            cout << "Moving " << thisChar->identify() 
                 << " from Room #" << r
                 << " to Room #" << newRoom << endl ;

            m_CharactersNextRound[newRoom].addNode(ptr) ;

         } else {

            m_CharactersNextRound[r].addNode(ptr) ;

         }
      }
   }


   // For each room and each Character in the room,
   // check if that character will change to a different
   // kind (e.g., Humans to Zombies).

   for (unsigned int r = 0 ; r < m_numRooms ; r++) {

      while ( ( ptr = m_CharactersNextRound[r].popNode() ) != NULL ) {
         thisChar = ptr->m_cptr ;
         otherChar = thisChar->morph() ;
         if (otherChar != NULL) {
            ptr->m_cptr = otherChar ;
            delete thisChar ;
         }
         m_CharactersInRoom[r].addNode(ptr) ;
      }         
   }

}


unsigned int Game::getRandomAdjacentRoom(unsigned int r) {

   unsigned int count = m_numAdjacent[r] ;
   if ( count == 0 ) return r ;

   unsigned int rand = (lrand48() % count) + 1;

   count = 0 ;
   for (unsigned int j = 0 ; j < m_numRooms ; j++) {
      if ( m_map[r][j] ) {
         count++ ;
         if (count == rand) return j ;
      }
   }

   cerr << "Error: something very wrong in getRandomAdjacentRoom().\n" ;
   return 0 ;
}


void Game::addToRoom(unsigned int r, Character *cptr)  {
   m_CharactersInRoom[r].add(cptr) ; 
}


unsigned int Game::numRooms() {
   return m_numRooms ; 
}


void Game::display() {
   Node *current ;

   for (unsigned int r = 0 ; r < m_numRooms ; r++) {
      cout << "In Room #" << r << endl ; 
      current = m_CharactersInRoom[r].m_header ;
      while (current->m_next != NULL) {
         current = current->m_next ;
         cout << current->m_cptr->identify() << endl ;
      }
      cout << endl ;
   }
}

