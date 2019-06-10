#ifndef PAIR_H
#define PAIR_H

template <class T> class Pair {
 public:
  Pair();
  Pair(T first, T second);
  T first() const;
  T second() const;
  bool operator==(const Pair& rhs) const;

 private:
  T m_first;
  T m_second;
};

#include "Pair.cpp"
#endif
