#ifndef PAIR_CPP
#define PAIR_CPP

#include "Pair.h"
#include <iostream>
using namespace std;

template <class T>
  Pair<T>::Pair() {

  }

template <class T>
  Pair<T>::Pair(T first, T second) {
    // : m_first(first), m_second(second){
    m_first = first;
    m_second = second;
  }

  template <class T>
  T Pair<T>::first() const {
    return m_first;
  }

  template <class T>
  T Pair<T>::second() const {
    return m_second;
  }

  template <class T>
  bool Pair<T>::operator==(const Pair<T>& rhs) const {
    //check if first==first and second==second
    if(m_first == rhs.first()  && m_second == rhs.second()) {
      //cout << "#1equal " << m_first << "==" << rhs.first() << " and "
      //	   << m_second << "==" << rhs.second() << endl;
      return true;
    }

    //check if first==second and second==first
    if(m_first == rhs.second()  && m_second == rhs.first()) {
      //cout << "#2equal " << m_first << "==" << rhs.second() << " and "
      //	   << m_second << "==" << rhs.first() << endl;
      return true;
    }

    return false;

  }

#endif
