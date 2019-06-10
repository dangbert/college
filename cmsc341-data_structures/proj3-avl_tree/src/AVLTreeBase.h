#ifndef _AVLTREEBASE_H_
#define _AVLTREEBASE_H_

class AVLTreeBase
{
  // dummy class required to remove template dependency
 public:

  /*
   * IN = in-order print
   * PRE = pre-order print
   * POST = post-order print
   * LEVEL = level-order print
   */
  enum PrintOrder{IN, PRE, POST, LEVEL};

  /*
   * types for template instantiations
   */
  enum DataType{CHARACTER, STRING};
  enum NumberType{INTEGER, FLOAT};
};

#endif /* _AVLTREEBASE_H_ */
