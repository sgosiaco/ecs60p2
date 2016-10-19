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


void InternalNode::addAndSort(BTreeNode* newNode)
{
  int i, value = newNode->getMinimum();

  newNode->setParent(this);

  for(i = count; i > 0 && keys[i - 1] > value; i--)
  {
    keys[i] = keys[i - 1];
    children[i] = children[i - 1];
  } // cycling through keys
  keys[i] = value;
  children[i] = newNode;
  count++;
  //cout << i << " = index and count = " << count << endl;
} // addAndSort()



void InternalNode::addToLeft(BTreeNode *left, BTreeNode *newNode)
{
  ((InternalNode *)left)->addAndSort(children[0]);

  for(int pos = 0; pos < count - 1; pos++)
  {
    keys[pos] = keys[pos + 1];
    children[pos] = children[pos + 1];
  }

  count--;
  addAndSort(newNode);
} //addToLeft()


void InternalNode::addToRight(BTreeNode *right, BTreeNode *newNode)
{
  BTreeNode* currNodeLargest = getMaximum();
  
  int rightMin = currNodeLargest->getMinimum();
  int newNodeMin = newNode->getMinimum();
  
  if(rightMin > newNodeMin)
  {
    ((InternalNode *)right)->addAndSort(currNodeLargest);
    count--;
    addAndSort(newNode);
    
  } // if largest node in current internal is bigger than the incoming node
  if(rightMin < newNodeMin)
    ((InternalNode *)right)->addAndSort(newNode);
} //addToRight()


void InternalNode::equalSplit(BTreeNode *node, BTreeNode *newInternal)
{ // should have access to original node count, left, right, etc.
  int half = internalSize/2, newKey = node->getMinimum();

  if(newKey < keys[half])
  {
    for(int i = half; i < internalSize; i++)
    {
      ((InternalNode *)newInternal)->addAndSort(children[i]);
      count--;
    }
    addAndSort(node);
  } // if a small value, goes to old array

  if(newKey > keys[half])
  {
    for(int i = half + 1; i < internalSize; i++)
    {
      ((InternalNode *)newInternal)->addAndSort(children[i]);
      count--;
    }
    ((InternalNode *)newInternal)->addAndSort(node);
  } //if a big value, go to new array
} // equalSplit()



void InternalNode::unequalSplit(BTreeNode *node, BTreeNode *newInternal)
{ // should have access to original node count, left, right, etc.
  int half = internalSize/2, newKey = node->getMinimum();

  if(newKey < keys[half - 1])
  {
    for(int i = half - 1; i < internalSize; i++)
    {
      ((InternalNode *)newInternal)->addAndSort(children[i]);
      count--;
    }
    addAndSort(node);
  } // if a small value, goes to old array

  if(newKey > keys[half - 1])
  {
    for(int i = half; i < internalSize; i++)
    {
      ((InternalNode *)newInternal)->addAndSort(children[i]);
      count--;
    }
    ((InternalNode *)newInternal)->addAndSort(node);
  } // if a big value, go to new array
} // unequalSplit()



int InternalNode::getMinimum()const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()



BTreeNode* InternalNode::getMaximum() const
{
  if(count > 0)
    return children[count - 1];
  else 
    return NULL;
} // InternalNode::getMaximum()



InternalNode* InternalNode::insert(int value)
{
  int pos; //cycle through the keys to find which child to insert in
  BTreeNode *newNode, *left = getLeftSibling(), *right = getRightSibling();

  for(pos = count - 1; pos > 0 && keys[pos] > value; pos--);
  newNode = children[pos]->insert(value);

  if(newNode)
  {
    if(count == internalSize) //check if internal node is full
    {
      if((left && left->getCount() < internalSize) || 
        (right && right->getCount() < internalSize))
          insert(newNode);
      else
      {
        InternalNode *newInternal = new InternalNode(internalSize, leafSize,
          NULL, this, right);
      
        if(count % 2 == 0) // put nodes into old and new internal nodes
          unequalSplit(newNode, newInternal);
        else // new leaf has equal number of values
          equalSplit(newNode, newInternal);

        rightSibling = newInternal;

        if(right) // adjust sibling pointer
          right->setLeftSibling(newInternal);
        
        updateKeys();
        newInternal->updateKeys();
        return newInternal; 
      } // if full on both sides, split!
    } // too many keys in internal node, 3 options
    else
      addAndSort(newNode);
  } // add on the key and point to new child 
  updateKeys();
  
  if(right)
    ((InternalNode *)right)->updateKeys();
  if(left)
    ((InternalNode *)left)->updateKeys();

  return NULL; // to avoid warnings for now.
} // InternalNode::insert()



void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{
  //cout << "a new root\n";
  keys[count] = oldRoot->getMinimum();
  children[count++] = oldRoot;
  keys[count] = node2->getMinimum();
  children[count++] = node2; 
  oldRoot->setParent(this);
  node2->setParent(this);
} // InternalNode::insert()


void InternalNode::insert(BTreeNode *newNode) // from a sibling
{
  BTreeNode *left = getLeftSibling(), *right = getRightSibling();

  if(left && left->getCount() < internalSize)
    addToLeft(left, newNode); 

  else
    addToRight(right, newNode);  
} // InternalNode::insert(BTreeNode *newNode)


void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  //cout << "\ncount = " << count;
  cout << endl;

  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()


void InternalNode::updateKeys()
{
  for(int pos = 0; pos < count; pos++)
  {
    int min = children[pos]->getMinimum();
    if(keys[pos] != min)
      keys[pos] = min;
  } // if the min value of the child changes
} // updateKeys()
