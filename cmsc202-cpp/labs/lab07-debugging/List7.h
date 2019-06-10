#ifndef LIST_H
#define LIST_H

using namespace std;

// The node used in List
class Node
{
   public:
   Node(int data);

   int m_data;
   Node* next;
};

// List is a linked list of ints
class List
{
   public:

   // Creates a default empty list
   List();
   
   // Creates a copy of another list
   List(const List &rhs);
   
   // Destructor
   ~List();

   // Assignment operator
   const List& operator=(const List &rhs);

   // Insert "data" into the list
   void insert(int data);
   
   // Remove "data" from the list.
   // Returns true on success, false on failure
   bool remove(int data);

   // Prints the list
   void Print() const;
   
   // Prints the list in reverse order
   void ReversePrint() const;

   // Deletes all the elements in the list
   void clear();

   // Returns the size of the list
   unsigned int size() const;

   private:

   Node* m_head;

};



#endif
