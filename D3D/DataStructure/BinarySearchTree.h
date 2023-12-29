#pragma once

#include "stdafx.h"

template<typename T>
class BinarySearchTree
{
public:
	struct Node;

public:
	BinarySearchTree() = default;
	~BinarySearchTree() = default;

	void Insert(Node** parent, Node* child)
	{
		if ((*parent)->Data < child->Data)
		{
			if ((*parent)->Right == nullptr)
				(*parent)->Right = child;
			else
				Insert(&(*parent)->Right, child);
		}

		else if ((*parent)->Data > child->Data)
		{
			if ((*parent)->Left == nullptr)
				(*parent)->Left = child;
			else
				Insert(&(*parent)->Left, child);
		}
	}

	void InOrder(Node* node)
	{
		if (node == nullptr) return;

		InOrder(node->Left);			//L
		cout << node->Data << ", ";		//r
		InOrder(node->Right);			//R
	}

public:
	static Node* CreateNode(T data)
	{
		Node* node = new Node();

		node->Data = data;
		node->Left = nullptr;
		node->Right = nullptr;

		return node;
	}

	static void DestroyNode(Node* node)
	{
		SafeDelete(node);
	}


	void Root(Node* newRoot) { root = newRoot; }

public:
	struct Node
	{
		T Data;
		Node* Left;
		Node* Right;
	};


private:
	Node* root;

};