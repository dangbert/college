#ifndef _GAME_H_
#define _GAME_H_

using namespace std ;

class Character ;

#include "linked.h"

class Game {

public:
   Game() ;
   Game(unsigned int n, bool *map, long seed) ;
   ~Game() ;

   // main driver function of Game class
   void playOneTurn() ;

   unsigned int getRandomAdjacentRoom(unsigned int r) ;

   // add given Character to room #r
   void addToRoom(unsigned int r, Character *cptr) ;

   // returns m_numRooms
   unsigned int numRooms() ;

   // prints the characters present in each room
   void display() ;

private:

   // number of rooms in this dungeon
   unsigned int m_numRooms ;

   // graph of the rooms in this dungeon
   // stored as an adjancency matrix.
   bool **m_map ;

   // remembers number of neighbors of each room
   unsigned int *m_numAdjacent ;

   // array of LinkedList. Each linked list
   // is a list of Characters currently occupying
   // the room. For example, m_CharactersInRoom[3]
   // is a list of Characters in room 3.
   LinkedList *m_CharactersInRoom ; 

   // temporary array to save the Characters that
   // are moving from one room to another.
   LinkedList *m_CharactersNextRound ;

} ;

#endif 
