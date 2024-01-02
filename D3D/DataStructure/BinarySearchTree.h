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

	Node* Search(Node* node, T data, int& depth)
	{
		if (node == nullptr) return nullptr;
		depth++;

		if (node->Data == data)
			return node;

		else if (node->Data > data)
			return Search(node->Left, data, depth);

		else if (node->Data < data)
			return Search(node->Right, data, depth);

		return nullptr;
	}

	Node* SearchMin(Node* node)
	{
		if (node == nullptr) return nullptr;

		if (node->Left == nullptr)
			return node;
		else
			return SearchMin(node->Left);
	}

	Node* Remove(Node* node, Node* parent, T data)
	{
		Node* remove = nullptr;

		if (node == nullptr) return nullptr;

		//Search
		if (node->Data > data)
			remove = Remove(node->Left, node, data);

		else if (node->Data < data)
			remove = Remove(node->Right, node, data);
		
		//Find
		else
		{
			remove = node;

			//No Child
			if (node->Left == nullptr && node->Right == nullptr)
			{
				if (parent->Left == node)
					parent->Left = nullptr;
				else
					parent->Right = nullptr;
			}
			//Exist Children
			else
			{
				//2 Children
				if (node->Left != nullptr && node->Right != nullptr)
				{
					Node* minNode = SearchMin(node->Right);

					minNode = Remove(node, nullptr, minNode->Data);
					node->Data = minNode->Data;

					return minNode;
				}

				//1 Child
				else
				{
					Node* temp = nullptr;

					if (node->Left != nullptr)
						temp = node->Left;
					else
						temp = node->Right;

					if (parent->Left == node)
						parent->Left = temp;
					else
						parent->Right = temp;

				}

			}
		}

		return remove;

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