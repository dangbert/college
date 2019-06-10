#include <iostream>
#include <cmath> //needed for pow()
#include "Poly.h"

using namespace std;


//The default constructor should set up the host object as an empty linked list, containing only a "dummy" node.
Polynomial::Polynomial(){
  m_head = new Node();
}


//The copy constructor creates a new Polynomial object from an existing one; it must make a "deep" copy.
Polynomial::Polynomial(const Polynomial& p){
  m_head = new Node();
  Node* current = p.m_head->m_next;

  while(current != NULL){
    insertMonomial(current->m_coefficient, current->m_degree);
    current = current->m_next; //advance to next node
  }
}


//The destructor must delete all the nodes of the linked list in a Polynomial object.
Polynomial::~Polynomial(){
  ///cout << "in destructor" << endl;
  if(m_head == NULL || m_head->m_next == NULL){
    cout << "The list is already empty" << endl;
    return; //exit
  }
  Node* current = m_head->m_next; //start at the first node after m_head
  Node* temp;
  m_head->m_next = NULL; //m_head now points to nothing

  while(current->m_next != NULL){ //continue through nodes (deleting as we go) until we get to the last node
    temp = current; //store the location of the node to be deleted
    current = current->m_next; //advance forward one node. The syntax is the same as (*current.m_next)
    delete temp; //delete the node
  }  

  delete current; //delete the last node
}


/*Inserts the monomial (coeff x^deg) into the linked list of the host object. If a monomial of the given degree is already present, it prints a warning message and leaves the host object unchanged.*/
void Polynomial::insertMonomial(long coeff, unsigned int deg){
  if(coeff==0)
    return;

  if(m_head->m_next == NULL){ //when the list is empty (just m_head)
    m_head->m_next = new Node(coeff, deg); //add the new node to the list
    return;
  }

  Node* current = m_head;
  Node* prev;

  while(current->m_next != NULL){ //go through all elements in the list
    if(deg > current->m_next->m_degree){ //if the next node is a lower degree than that being inserted
      Node* temp = current->m_next; //keep track of the node that will go after the new one
      current->m_next = new Node(coeff, deg); //connect the node before the new one, to it
      current->m_next->m_next = temp; //connect the new node to the one after it
      return;
    }

    if(deg == current->m_next->m_degree){
      cout << "Warning: Monomial of degree " << deg << " already present!" << endl;
      return;
    }
    current = current->m_next; //advance to the next node in the list
  }

  current->m_next = new Node(coeff, deg); //if the last node is reached and none were of lower degree
}


/*Returns the mathematical sum of the host object and p. The implementation must be efficient, requiring only a single pass through each linked list.*/
Polynomial Polynomial::add(const Polynomial& p) const{
  Polynomial s; //polynomial to store the sum
  Node* current1 = m_head->m_next; ///for now assume both are nonempty (deg!=0)
  Node* current2 = p.m_head->m_next;//p.get_head()->m_next;
  bool c1_complete = false;
  bool c2_complete = false;

  while(!(c1_complete && c2_complete)){

    //case: deg_1 > deg_2
    if((current1->m_degree > current2->m_degree && !c1_complete) || (c2_complete && !c1_complete) ){
      s.insertMonomial(current1->m_coefficient, current1->m_degree);
      if(current1->m_next != NULL)
	current1=current1->m_next;
      else{
	c1_complete = true;
      }
    }

    //case: deg_2 > deg_1
    if(current2->m_degree > current1->m_degree && !c2_complete || (c1_complete && !c2_complete)){
      s.insertMonomial(current2->m_coefficient, current2->m_degree);
      if(current2->m_next != NULL)
	current2=current2->m_next;
      else{
	c2_complete = true;
      }
    }
    
    //case: deg_1 == deg_2
    if(current1->m_degree == current2->m_degree){     
      s.insertMonomial(current1->m_coefficient + current2->m_coefficient, current1->m_degree);
      if(current1->m_next != NULL)
	current1=current1->m_next;
      else{
	c1_complete = true;
      }
      if(current2->m_next != NULL)
	current2=current2->m_next;
      else{
	c2_complete = true;
      }
    }

  }
  return s;
}


/*Returns the mathematical difference of the host object and p (host - p). The implemenation must be efficieint (see add(), above).*/
Polynomial Polynomial::subtract(const Polynomial& p) const{
  Polynomial s; //polynomial to store the difference

  Node* current1 = m_head->m_next; ///for now assume both are nonempty (deg!=0)
  Node* current2 = p.m_head->m_next;
  bool c1_complete = false;
  bool c2_complete = false;

  while(!(c1_complete && c2_complete)){

    //case: deg_1 > deg_2
    if((current1->m_degree > current2->m_degree && !c1_complete) || (c2_complete && !c1_complete) ){
      s.insertMonomial(current1->m_coefficient, current1->m_degree);
      if(current1->m_next != NULL)
	current1=current1->m_next;
      else{
	c1_complete = true;
      }
    }

    //case: deg_2 > deg_1
    if(current2->m_degree > current1->m_degree && !c2_complete || (c1_complete && !c2_complete)){
      s.insertMonomial(-1 * current2->m_coefficient, current2->m_degree);
      if(current2->m_next != NULL)
	current2=current2->m_next;
      else{
	c2_complete = true;
      }
    }
    
    //case: deg_1 == deg_2
    if(current1->m_degree == current2->m_degree){     
      s.insertMonomial(current1->m_coefficient - current2->m_coefficient, current1->m_degree);
      if(current1->m_next != NULL)
	current1=current1->m_next;
      else{
	c1_complete = true;
      }
      if(current2->m_next != NULL)
	current2=current2->m_next;
      else{
	c2_complete = true;
      }
    }

  }
  return s;
}


//Returns the mathematical product of the host polynomial and p.
Polynomial Polynomial::multiply(const Polynomial& p) const{
  Polynomial s;
  if(m_head->m_next == NULL || p.m_head->m_next == NULL)
    return s; //return empty polynomial

  Node* c1 = m_head->m_next;
  Node* c2 = p.m_head->m_next;
  
  while(c1 != NULL){ //step through all of c1, multiplying each monomial by all of c2

    while(c2 != NULL){ //add the product of each monomial to s
      s.addTo(c1->m_coefficient * c2->m_coefficient, c1->m_degree + c2->m_degree);
      c2=c2->m_next;
    }

    c2=p.m_head->m_next;
    c1=c1->m_next;
  }

  return s;
}


//Returns the remainder when dividing the host polynomial by p.
Polynomial Polynomial::modulo(const Polynomial& p) const{  // Extra Credit

}


//Returns the degree (highest power of x) of the host object.
unsigned int Polynomial::degree() const{
  if(m_head->m_next == NULL)
    return 0; //return 0 when the polynomial is empty
  else
    return m_head->m_next->m_degree; //degree of first monomial in node list
}


//Evaluates the polynomial for the given value of x.
long Polynomial::evaluate(long x) const{
  long r=0;
  Node* current = m_head->m_next;

  if(m_head->m_next == NULL)
    return 0;

  while(current!= NULL){
    r += current->m_coefficient * pow(x, current->m_degree);

    current = current->m_next;
  }
  return r;
}

//The overloaded assignment operator performs a "deep copy" of p.
///Polynomial& operator=(const Polynomial& p);
Polynomial& Polynomial::operator=(const Polynomial &p){
  //clear the host's list
  if(!(m_head == NULL || m_head->m_next == NULL)){
    Node* current = m_head->m_next; //start at the first node after m_head
    Node* temp;
    m_head->m_next = NULL; //m_head now points to nothing

    while(current->m_next != NULL){ //continue through nodes (deleting as we go) until we get to the last node
    
      temp = current; //store the location of the node to be deleted
      current = current->m_next; //advance forward one node. The syntax is the same as (*current.m_next)
    
      delete temp; //delete the node
    }  
    delete current;
  }


  Node*current = p.m_head->m_next;
  //now copy p to the host
  while(current != NULL){
    insertMonomial(current->m_coefficient, current->m_degree);
    current = current->m_next; //advance to next node
  }
  //***what do i return???
}

/*The overloaded insertion operator can be used to display a Polynomial object in standard mathematical notation. See the test output for a sample of acceptable output.*/

ostream& operator<<(ostream& sout, const Polynomial& p){
  Node* current = p.m_head->m_next;
  
  while(current != NULL){
    sout << current->m_coefficient << "x^" << current->m_degree;
    current = current->m_next; //advance to next node
    if(current != NULL)
      sout << " + ";
      }
  sout << endl;
}

//see description in Poly.h
void Polynomial::addTo(long coeff, unsigned int deg){
  if(coeff==0)
    return;

  Node* current = m_head;
  Node* prev;

  while(current->m_next != NULL){ //go through all elements in the list
    if(deg > current->m_next->m_degree){ //if the next node is a lower degree than that being inserted
      Node* temp = current->m_next; //keep track of the node that will go after the new one
      current->m_next = new Node(coeff, deg); //connect the node before the new one, to it
      current->m_next->m_next = temp; //connect the new node to the one after it
      return;
    }

    if(deg == current->m_next->m_degree){ //what we care about
      Node * temp = current->m_next;
      current->m_next->m_coefficient = current->m_next->m_coefficient + coeff;
      if(current->m_next->m_coefficient == 0){

      }
      return;
    }
    current = current->m_next; //advance to the next node in the list
  }

  current->m_next = new Node(coeff, deg); //if the last node is reached and none were of lower degree
}
