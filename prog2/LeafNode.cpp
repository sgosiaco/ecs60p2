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


void LeafNode::addAndSort(int value)
{
  int i;
  for(i = count; i > 0 && values[i - 1] > value; i--)
    values[i] = values[i - 1];
  values[i] = value;
  count++;
  //cout << i << " = index and count = " << count << endl;
} // addAndSort()


void LeafNode::addToLeft(BTreeNode *left, int value)
{
  int minimum = getMinimum();
  ((LeafNode *)left)->addAndSort(minimum);

  for(int pos = 0; pos < count - 1; pos++)
    values[pos] = values[pos + 1];

  count--;
  addAndSort(value);
} //addToLeft()


void LeafNode::addToRight(BTreeNode *right, int value)
{
  int maximum = getMaximum();

  if(maximum > value)
  {
    ((LeafNode *)right)->addAndSort(maximum);
    count--;
    addAndSort(value);
  }

  if(maximum < value)
    ((LeafNode *)right)->addAndSort(value);
} //addToRight() 


void LeafNode::equalSplit(LeafNode& newLeaf, int value)
{ // should have access to original node count, left, right, etc.
  int half = leafSize/2;

  if(value < values[half])
  {
    for(int i = half; i < leafSize; i++)
    {
      newLeaf.addAndSort(values[i]);
      count--;
    }
    addAndSort(value);
  } // if a small value, goes to old array

  if(value > values[half])
  {
    for(int i = half + 1; i < leafSize; i++)
    {
      newLeaf.addAndSort(values[i]);
      count--;
    }
    newLeaf.addAndSort(value);
  } //if a big value, go to new array
} // equalSplit()



void LeafNode::unequalSplit(LeafNode& newLeaf, int value)
{ // should have access to original node count, left, right, etc.
  int half = leafSize/2;

  if(value < values[half - 1])
  {
    for(int i = half - 1; i < leafSize; i++)
    {
      newLeaf.addAndSort(values[i]);
      count--;
    }
    addAndSort(value);
  } // if a small value, goes to old array

  if(value > values[half - 1])
  {
    //cout << "big split\n";
    for(int i = half; i < leafSize; i++)
    {
      newLeaf.addAndSort(values[i]);
      count--;
    }
    newLeaf.addAndSort(value);
  } // if a big value, go to new array
} // unequalSplit()



int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;
} // LeafNode::getMinimum()


int LeafNode::getMaximum() const
{
  if(count > 0)
    return values[count - 1];
  else
    return 0;
}

LeafNode* LeafNode::insert(int value)
{
  if(count == leafSize) 
  {
    BTreeNode *left = getLeftSibling(), *right = getRightSibling();

    if(left && left->getCount() < leafSize)
      addToLeft(left, value);  
    else
    {
      if(right && right->getCount() < leafSize)
        addToRight(right, value);  
      else
      {
        LeafNode *newLeaf = new LeafNode(leafSize, NULL, this, right);
        
        if(count % 2 == 0) // new leaf has more values than original
          unequalSplit(*newLeaf, value);
        else // new leaf has equal number of values
          equalSplit(*newLeaf, value);

        rightSibling = newLeaf;

        if(right) // adjust sibling pointer
          right->setLeftSibling(newLeaf);

        return newLeaf; 
      } // if full on both sides, split!
    }
  } // 3 options: give to left, give to right, split
  else
    addAndSort(value);
  return NULL; // to avoid warnings for now.
}  // LeafNode::insert()



void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()
