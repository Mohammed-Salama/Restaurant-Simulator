#pragma once 
#include "Queue.h"
template <class T, bool func(T a, T b)>
class Priority_Queue : public Queue<T>
{
public:
	virtual  bool enqueue(const T& newEntry);
};

//test of using visual studio 
template <class T, bool func(T a, T b)>                       
bool Priority_Queue<T,func>::enqueue( const T& newEntry)
{
	Node<T>* newNodePtr = new Node<T>(newEntry);
	if (isEmpty())
	{
		frontPtr = newNodePtr; 
		backPtr = newNodePtr; 
		return true;
	}
	if(func(newEntry , frontPtr->getItem()))
	{
		newNodePtr->setNext(frontPtr);
		frontPtr = newNodePtr;
		return true;
	}
	Node<T>* ptr = frontPtr;
	while(ptr->getNext() && func(ptr->getNext()->getItem() , newEntry))
		ptr = ptr->getNext();
	newNodePtr->setNext(ptr->getNext());
	ptr->setNext(newNodePtr);
	if(backPtr == ptr)
		backPtr = newNodePtr;
	return true ;
}
