#pragma once

#include "stdafx.h"

template<typename T>
class BTree
{
public:
	struct Node;

public:
	BTree() = default;

	~BTree()
	{
		DestroyAllNode(root);
	}

	void DestroyAllNode(Node* node)
	{
		if (node == nullptr) return;

		DestroyAllNode(node->Left); //L
		DestroyAllNode(node->Right); //R
		DestroyNode(node); //r
	}

	void Root(Node* newRoot) { root = newRoot; }

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

public:
	void PreOrder(Node* node)
	{
		if (node == nullptr) return;

		cout << " " << node->Data; //r
		PreOrder(node->Left); //L
		PreOrder(node->Right); //R
	}

	void InOrder(Node* node)
	{
		if (node == nullptr) return;

		InOrder(node->Left); //L
		cout << " " << node->Data; //r
		InOrder(node->Right); //R
	}

	void PostOrder(Node* node)
	{
		if (node == nullptr) return;

		PostOrder(node->Left); //L
		PostOrder(node->Right); //R
		cout << " " << node->Data; //r
	}


private:
	struct Node
	{
		T Data;

		Node* Left;
		Node* Right;
	};

	

private:
	Node* root;
};