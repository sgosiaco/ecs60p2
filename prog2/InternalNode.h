#ifndef InternalNodeH
  #define InternalNodeH

#include "BTreeNode.h"
#include "LeafNode.h"

class InternalNode:public BTreeNode
{
  int internalSize;
  BTreeNode **children;
  int *keys;
public:
  InternalNode(int ISize, int LSize, InternalNode *p,
    BTreeNode *left, BTreeNode *right);
  void addAndSort(BTreeNode *newNode);
  void addToLeft(BTreeNode *left, BTreeNode *newNode);
  void addToRight(BTreeNode *right, BTreeNode *newNode);
  void equalSplit(BTreeNode *node, BTreeNode *newInternal);
  void unequalSplit(BTreeNode *node, BTreeNode *newInternal);
  int getMinimum()const;
  BTreeNode* getMaximum()const;
  InternalNode* insert(int value); // returns ptr to InternalNode, else NULL
  void insert(BTreeNode *oldRoot, BTreeNode *node2); // if root splits use this
  void insert(BTreeNode *newNode); // from a sibling
  void print(Queue <BTreeNode*> &queue);
  void updateKeys();
}; // InternalNode class

#endif
