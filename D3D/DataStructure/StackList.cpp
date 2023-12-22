#include "stdafx.h"
#include "StackList.h"

#include <queue>

StackList::~StackList()
{

}

void StackList::Push(StackNode::Node* newNode)
{
	if (list == nullptr)
	{
		list = newNode;
	}
	else
	{
		StackNode::Node* current = list;

		while (current->Next != nullptr)
			current = current->Next;

		current->Next = newNode;

		top = newNode;
	}
}

StackNode::Node* StackList::Pop()
{
	StackNode::Node* pop = top;

	if (list == top)
	{
		list = nullptr;
		top = nullptr;
	}
	else
	{
		StackNode::Node* tail = list;
		while (tail != nullptr && tail->Next != top)
			tail = tail->Next;

		top = tail;
		tail->Next = nullptr;
	}

	return pop;
}

int StackList::Size()
{
	int count = 0;

	StackNode::Node* current = list;

	while (current != nullptr)
	{
		current = current->Next;
		count++;
	}

	return count;
}

StackNode::Node* StackList::CreateNode(string data)
{
	StackNode::Node* node = new StackNode::Node();
	
	node->Data = data;
	node->Next = nullptr;

	return node;
}

void StackList::DestroyNode(StackNode::Node* node)
{
	SafeDelete(node);
}
