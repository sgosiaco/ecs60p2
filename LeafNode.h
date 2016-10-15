#ifndef LeafNodeH
#define LeafNodeH

#include "BTreeNode.h"

class LeafNode:public BTreeNode
{
  int *values;
public:
  LeafNode(int LSize, InternalNode *p, BTreeNode *left,
    BTreeNode *right);
  int getMinimum() const;
  LeafNode* insert(int value); // returns pointer to new Leaf if splits else NULL
  void print(Queue <BTreeNode*> &queue);

  void add(int value);
  void addFull(int value, int &last);
  void addLeft(int value, int last);
  void addRight(int value, int last);
  LeafNode* split(int value, int last);
}; //LeafNode class

#endif
