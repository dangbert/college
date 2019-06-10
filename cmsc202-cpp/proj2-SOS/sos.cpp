#include <iostream>
#include <algorithm>
#include "sos.h"
using namespace std;

SoS::SoS() //default constructor
   {
   m_cofinite = false;
   }


/* INSERT:
This function should add the string str to the set represented by the host object
You must not have duplicate values stored in m_vos. This is after all a set.
Also, you may be inserting str into a cofinite set. In this case, you would want
to remove str from m_vos if it is there. Although the vector class does support an
erase() method, it is simpler to construct a new vector of strings that omits str
and assign the new vector to m_vos. */
void SoS::insert(string str)
   {
   if(!isMember(str))
     {
     if(!m_cofinite) //when the set is finite and doesn't conatin str
       m_vos.push_back(str);

     else //when str is not a member of a confinite set (and thus in m_vos)
       {
       int pos = find(m_vos.begin(), m_vos.end(), str) - m_vos.begin(); //location of str in m_vos
       if(pos < m_vos.size()) //check that pos is in bounds 
	 {
	 // cout << m_vos[pos] << " " << " is  located at " <<  pos << endl;
	 m_vos.erase(m_vos.begin() + 1);
	 }
       else
	 cout << pos << " is out of bounds" << endl;

       }

     }
   }


/* PRINT:
For finite sets, print out the strings in the set, one per line. For cofinite sets,
print out "COMPLEMENT OF:" followed by the strings in m_vos, one per line. */
void SoS::print() const 
   {
   if(m_cofinite)
     cout << "COMPLEMENT OF:" << endl;
      
   for(int i=0; i< m_vos.size(); i++)
      {
      cout << m_vos[i] << endl; //print one string per line
      }
   cout << endl;
   }


/* isMember:
Return true if str is a member of the set represented by the host object. Return
false otherwise. Do take care of the case where the host object is representing a
cofinite set. */
bool SoS::isMember(string str) const 
   {//***shorten this code later
     if(m_cofinite == false) //if the set it finite
       {//***replace this with find()
      for(int i=0; i<m_vos.size(); i++)
         {
         if(m_vos[i] == str) //if str is in m_vos, return true
            return true;
         }
      return false;
      }
      
   else //when the set is cofinite
     {
      for(int i=0; i<m_vos.size(); i++)
         {
         if(m_vos[i] == str) //if str is in m_vos
            return false; //return false b/c m_vos is confinite
         }
      return true;
     }
   }

/* isSubset:
Return true if the host object represents a set that is a subset of the set represented by B.*/
bool SoS::isSubset(const SoS& B) const
   {
     //case1: both finite
     if(isFinite() && B.isFinite())
       {
	 if(m_vos.size() > B.getSize())
	   return false;

	 for(int i=0; i<m_vos.size(); i++) //check if each member of the host object is a member of b
	   {
	     if(m_vos[i] != B.getElement(i));
		return false;
	   }
	 return true;
       }

     //case2: host finite, B cofinite
     if(isFinite() && !B.isFinite())
       {
         for(int i=0; i<m_vos.size(); i++)
           {
             if(m_vos[i] != B.getElement(i));
                return false;
	   }
	 return true;
       }

     //case3: host cofinite, B finite
     if(!isFinite() && B.isFinite())
	 return false;

     //case 4: both cofinite
     if(!isFinite() && !B.isFinite())
       {
	 for(int i=0; i<m_vos.size(); i++) //check that every nonmember of host is a nonmember of B
	   {
	     //SoS comp = B.makeComplement();
	     if(!B.makeComplement().isMember(m_vos[i]))
		return false;
	   }
       return true;
       }
   }


/* isFinite:
Return true if the host object is a finite set and false otherwise. */   
bool SoS::isFinite() const
   {
   if(!m_cofinite)
      return true; //return true when the set is finite
      
   return false; //return false when the set is no finite
   }


/*makeComplement:
Return an SoS object that is the complement of the host object. Note that you cannot
just invert m_cofinite because that does not return an object. Also, makeComplement()
is a const member function, so you cannot modify m_cofinite anyway. So, you must make
a copy of the host with m_cofinite flipped and return that. (Yes, it is OK to return
a local object because it gets copied during the return.)*/
SoS SoS::makeComplement() const
   {
     SoS c;
     for(int i=0; i<m_vos.size(); i++)
       c.insert(m_vos[i]);

     c.reverseSetType();
     return c;
   }
   
   
/* makeUnion:
Return an SoS object that is the union of the host object and B. Again, you must
create a new object. As with isSubset(), you need to consider the four cases where
the host object and B are finite and cofinite. Draw lots of Venn diagrams. You should
not have any duplicate strings stored in m_vos member of the new SoS object. */   
SoS SoS::makeUnion(const SoS& B) const
   {
   SoS u; //creates the SoS object u that will represent the union of the host and B
   
   //case 1: both finite
   if(isFinite() && B.isFinite())
     {
     for(int i=0; i<B.getSize(); i++)
       u.insert(B.getElement(i)); //insert unsures no duplicate is added

     for(int j=0; j<m_vos.size(); j++)
       u.insert(m_vos[j]);

     }
   
   //case 2: host finite, B cofinite
   if(isFinite() && !B.isFinite())
     {

     //in this case u will be cofinite.
     //thus u's m_vos must contain the strings in B's m_vos that also aren't in the host's m_vos
     //u won't be set as cofinite until the end because elements can't be added to the complement of an empty set
     for(int i=0; i<B.getSize(); i++) //loop through all elements in B's m_vos
       {
       if(!isMember(B.getElement(i))) //if the particular element in B's m_vos is NOT in the host's m_vos
	 u.insert(B.getElement(i)); //add the element to u's m_vos
       }

     u.reverseSetType(); //now that u's m_vos is set, make u cofinite
     }

   //case 3: host cofinite, B finite
   if(!isFinite() && B.isFinite())
     {
     //in this case u will be cofinite.
     //thus u's m_vos must contain the strings in the host's m_vos that also aren't in the B's m_vos
     //u won't be set as cofinite until the end because elements can't be added to the complement of an empty set
     for(int i=0; i<m_vos.size(); i++) //loop through all elements in the host's m_vos
       {
       if(!B.isMember(m_vos[i])) //if the particular element in the host's m_vos is NOT in the B's m_vos
	 u.insert(m_vos[i]); //add the element to u's m_vos
       }

       u.reverseSetType(); //now that u's m_vos is set, make u cofinite
     }

   //case 4: both cofinite
   if(!isFinite() && !B.isFinite())
     {
     //in this case u will be cofinite.
     //thus u's m_vos must contain the strings in the INTERSECTION of host's m_vos and B's m_vos
     //u won't be set as cofinite until the end because elements can't be added to the complement of an empty set
     for(int i=0; i<m_vos.size(); i++) //loop through all elements in the host's m_vos (these aren't in its set)
       { //even if B.getSize() is > m_vos.size(), the additional elements in B's m_vos don't need to be checked
       if(!B.isMember(m_vos[i])) //if the particular element is in both the host's m_vos and B's m_vos
	 u.insert(m_vos[i]); //add the element to u's m_vos
       }

       u.reverseSetType(); //now that u's m_vos is set, make u cofinite
     }



   return u;
   }


/* makeIntersection:
Same as makeUnion() except this time you want the intersection. Again, draw lots of
Venn diagrams. You should not have any duplicate strings stored in m_vos member of
the new SoS object. */  
SoS SoS::makeIntersection(const SoS& B) const
   {
   SoS inter;

   //case 1: both finite
   if(isFinite() && B.isFinite())
     {
       for(int i=0; i<m_vos.size(); i++)
	 {
	   if(B.isMember(m_vos[i]))
	      inter.insert(m_vos[i]); //add to i if its a member of both sets
	 }

     }

   //case2: host finite, B cofinite
   if(isFinite() && !B.isFinite())
     {
       for(int i=0; i<m_vos.size(); i++)
	 {
	   if(B.isMember(m_vos[i]))
	     {
	       inter.insert(m_vos[i]); //add element to i if its a member of both sets
	     }
	 }
     }

   //case 3: host cofinite, B finite
   if(!isFinite() && B.isFinite())
     {
       for(int i=0; i<B.getSize(); i++)
         {
           if(isMember(B.getElement(i)))
             {
               inter.insert(B.getElement(i)); //add element to i if its a member of both sets
             }
         }
     }

   //case4: both cofinite
   if(!isFinite() && !B.isFinite())
     {
       for(int i=0; i<m_vos.size(); i++)
	   inter.insert(m_vos[i]);

       for(int j=0; j<B.getSize(); j++)
	   inter.insert(B.getElement(j));
     }

   inter.reverseSetType();
   return inter;
   }


void SoS::reverseSetType()
{
  m_cofinite = !m_cofinite;  
}

//find(vec.begin(),vec.end(),"r")
int SoS::getSize() const
{
  return m_vos.size();
}

string SoS::getElement(int i) const
{
  if(i < getSize()) 
    return m_vos[i];
  return "uhhhh";
}
