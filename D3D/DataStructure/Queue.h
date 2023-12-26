#pragma once

#include "stdafx.h"

template<typename T>
class Queue
{
public:
	struct Node;

public:
	~Queue()
	{
		while(IsEmpty() == false)
			DestroyNode(Pop());
	}

public:
	static Node* CreateNode(T data)
	{
		Node* node = new Node();
		node->Data = data;
		node->Next = nullptr;

		return node;
	}

	static void DestroyNode(Node* node)
	{
		SafeDelete(node);
	}

public:
	void Push(Node* newNode)
	{
		if (front == nullptr)
		{
			front = newNode;
			rear = newNode;
			count++;

			return;
		}

		rear->Next = newNode;
		rear = newNode;

		count++;
	}

	Node* Pop()
	{
		Node* node = front;

		if (front->Next == nullptr)
			front = rear = nullptr;
		else
			front = front->Next;

		return node;
	}

	bool IsEmpty() { return front == nullptr; }
	int Count() { return count; }

private:
	struct Node
	{
		T Data;
		Node* Next;
	};

private:
	Node* front = nullptr;
	Node* rear = nullptr;

	int count = 0;
};