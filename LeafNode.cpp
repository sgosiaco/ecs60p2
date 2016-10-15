#include <iostream>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()



int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()

void LeafNode::add(int value)
{
  int i;
  for(i = count - 1; i >= 0 && value < values[i]; i--)
    values[i + 1] = values[i];

  values[i + 1] = value;
  count++;

  if(values[0] == value && parent)
    parent->updateMin(this);
}

void LeafNode::addLeft(int value, int last)
{
  int i;

  leftSibling->insert(values[0]);
  for(i = 0; i < count - 1; i++)
    values[i] = values[i + 1];

  values[i] = last;

  if(parent)
    parent->updateMin(this);
}

void LeafNode::addRight(int value, int last)
{
  rightSibling->insert(last);

  if(values[0] == value && parent)
    parent->updateMin(this);
}

void LeafNode::addFull(int value, int &last)
{
  int i;

  if(value > values[count - 1])
    last = value;
  else
  {
    last = values[count - 1];

    for(i = count - 2; i >= 0 && value < values[i]; i--)
      values[i + 1] = values[i];

    values[i + 1] = value;
  }
}

LeafNode* LeafNode::insert(int value)
{
  int last;

  if(count < leafSize)
  {
    add(value);
    return NULL;
  }

  addFull(value, last);

  if(leftSibling && leftSibling->getCount() < leafSize)
  {
    addLeft(value, last);
    return NULL;
  }
  else //no room in left or it doesn't exist
    if(rightSibling && rightSibling->getCount() < leafSize)
    {
      addRight(value, last);
      return NULL;
    }
    else//gotta split
      return split(value, last);

  return NULL; // to avoid warnings for now.
}  // LeafNode::insert()

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()

LeafNode* LeafNode::split(int value, int last)
{
  LeafNode *temp = new LeafNode(leafSize, parent, this, rightSibling);

  if(rightSibling)
    rightSibling->setLeftSibling(temp);

  rightSibling = temp;

  for(int i = (leafSize + 1) / 2; i < leafSize; i++)
  {
    temp->values[temp->getCount()] = values[i];
    temp->count++;
  }

  temp->values[temp->getCount()] = last;
  temp->count++;
  count = (leafSize + 1) / 2;

  if(values[0] == value && parent)
    parent->updateMin(this);

  return temp;
}
