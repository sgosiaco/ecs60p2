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

void InternalNode::add(int value)
{
  int i;

  for(i = count - 1; i >= 0 && value < keys[i]; i--);

  BTreeNode* temp = children[i + 1]->insert(value);

  if(temp)
    insert(temp);
  //if(values[0] == value && parent)
    //parent->updateMin(this);
}


InternalNode* InternalNode::insert(int value)
{
  add(value);
  // students must write this
  return NULL; // to avoid warnings for now.
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{ // Node must be the root, and node1
  // students must write this
  keys[0] = oldRoot->getMinimum();
  children[0] = oldRoot;
  oldRoot->setParent(this);

  keys[1] = node2->getMinimum();
  children[1] = node2;
  node2->setParent(this);

  count = 2;
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *newNode) // from a sibling
{
  keys[++count] = newNode->getMinimum();
  children[count] = newNode;
  // students may write this
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

void InternalNode::updateMin(BTreeNode *node)
{
  int i;

  for(i = 0; i < count; i++)
    if(children[i] == node)
    {
      keys[i] = node->getMinimum();
      break;
    }
}
