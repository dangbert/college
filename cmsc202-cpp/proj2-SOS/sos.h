#ifndef SOS_H
#define SOS_H

#include <string>
#include <vector>
using namespace std;

class SoS {

   public:

   // Do not change the member function prototypes for
   // any public member function.

   // See documentation in Project 2 description.

   SoS() ;
   SoS(vector<string> v, bool co) ;

   void insert(std::string str) ;
   void print() const ;

   bool isMember(string str) const ;
   bool isSubset(const SoS& B) const ; 
   bool isFinite() const ;

   SoS makeComplement() const ;
   SoS makeUnion(const SoS& B) const ;
   SoS makeIntersection(const SoS& B) const ;
   
   
   // The function dump() is used for grading.  Do not modify! 
   vector<string>  dump() const { return m_vos; }


   private:

   // Do not change the types of these private data members

   bool m_cofinite ;         //true when the object represents a confinite set
   vector<string> m_vos ;    // vos = vector of strings (of the representation)


   // Declarations for Additional private member functions
   // may be added below. Fully document these.
   void reverseSetType();
   int getSize() const;
   string getElement(int i) const;
} ;

#endif
