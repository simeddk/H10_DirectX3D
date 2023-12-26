#pragma once

#include "stdafx.h"

template<typename T>
class Tree
{
public:
	struct Node;

public:
	static Node* CreateNode(T data)
	{
		Node* node = new Node();
		node->Data = data;
		node->Child = node->Sibling = nullptr;
		
		return node;
	}

	static void DestroyNode(Node* node)
	{
		SafeDelete(node);
	}

	void Add(Node* parent, Node* newChild)
	{
		if (parent->Child == nullptr)
		{
			parent->Child = newChild;
		}
		else
		{
			Node* node = parent->Child;
			while (node->Sibling != nullptr)
				node = node->Sibling;
			
			node->Sibling = newChild;
		}
	}

	void Print(Node* node, int depth)
	{
		for (int i = 0; i < depth; i++)
			printf("¡æ");

		cout << node->Data << endl;

		if (node->Child != nullptr)
			Print(node->Child, depth + 1);

		if (node->Sibling != nullptr)
			Print(node->Sibling, depth);
	}

private:
	struct Node
	{
		T Data;

		Node* Child;
		Node* Sibling;
	};
};