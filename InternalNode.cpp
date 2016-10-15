#include <iostream>
#include "InternalNode.h"

using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()


int InternalNode::getMinimum()const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()

void InternalNode::add(BTreeNode *ptr, int pos)
{
  int i;

  for(i = count - 1; i >= pos; i--)
  {
    children[i + 1] = children[i];
    keys[i + 1] = keys[i];
  }

  children[pos] = ptr;
  keys[pos] = ptr->getMinimum();
  count++;
  ptr->setParent(this);

  if(pos == 0 && parent)
    parent->updateMin(this);
}

void InternalNode::addLeft(BTreeNode *last)
{
  ((InternalNode*)leftSibling)->insert(children[0]);

  for(int i = 0; i < count - 1; i++)
  {
    children[i] = children[i + 1];
    keys[i] = keys[i + 1];
  }

  children[count - 1] = last;
  keys[count - 1] = last->getMinimum();
  last->setParent(this);
  if(parent)
    parent->updateMin(this);
}

void InternalNode::addRight(BTreeNode *ptr, BTreeNode *last)
{
  ((InternalNode*)rightSibling)->insert(last);
  if(ptr == children[0] && parent)
    parent->updateMin(this);
}

BTreeNode* InternalNode::addFull(BTreeNode *ptr, int pos)
{
  if(pos == internalSize)
    return ptr;

  BTreeNode *last = children[count - 1];

  for(int i = count - 2; i >= 0; i--)
  {
    children[i + 1] = children[i];
    keys[i + 1] = keys[i];
  }

  children[pos] = ptr;
  keys[pos] = ptr->getMinimum();
  ptr->setParent(this);
  return last;
}


InternalNode* InternalNode::insert(int value)
{
  int i;
  BTreeNode* last;

  for(i = count - 1; i > 0 && value < keys[i]; i--);

  BTreeNode* temp = children[i]->insert(value);
  if(!temp)
    return NULL;

  if(count < internalSize)
  {
    add(temp, i + 1);
    return NULL;
  }

  last = addFull(temp, i + 1);

  if(leftSibling && leftSibling->getCount() < internalSize)
  {
    addLeft(last);
    return NULL;
  }
  else //no room in left or it doesn't exist
    if(rightSibling && rightSibling->getCount() < internalSize)
    {
      addRight(temp, last);
      return NULL;
    }
    else//gotta split
      return split(last);


  return NULL; // to avoid warnings for now.
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{ // Node must be the root, and node1
  // students must write this
  keys[0] = oldRoot->getMinimum();
  children[0] = oldRoot;
  keys[1] = node2->getMinimum();
  children[1] = node2;

  children[0]->setLeftSibling(NULL);
  children[0]->setRightSibling(children[1]);
  oldRoot->setParent(this);
  children[1]->setLeftSibling(children[0]);
  children[1]->setRightSibling(NULL);
  node2->setParent(this);

  count = 2;

} // InternalNode::insert()

void InternalNode::insert(BTreeNode *newNode) // from a sibling
{
  keys[count] = newNode->getMinimum();
  children[count] = newNode;
  count++;

} // InternalNode::insert()

void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl;

  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()

void InternalNode::updateMin(const BTreeNode *node)
{
  int i;

  for(i = 0; i < count; i++)
    if(children[i] == node)
    {
      keys[i] = node->getMinimum();
      if(parent && i == 0)
        parent->updateMin(this);
      break;
    }
}
