#ifndef LeafNodeH
#define LeafNodeH

#include "BTreeNode.h"

class LeafNode:public BTreeNode
{
  int *values;
public:
  LeafNode(int LSize, InternalNode *p, BTreeNode *left,
    BTreeNode *right);
  void addAndSort(int value);
  void equalSplit(LeafNode& newLeaf, int value);
  void unequalSplit(LeafNode& newLeaf, int value);
  void addToLeft(BTreeNode *left, int value);
  void addToRight(BTreeNode *right, int value);
  int getMinimum() const;
  int getMaximum() const;
  LeafNode* insert(int value); // returns ptr to new Leaf if splits, else null
  void print(Queue <BTreeNode*> &queue);
}; //LeafNode class

#endif
