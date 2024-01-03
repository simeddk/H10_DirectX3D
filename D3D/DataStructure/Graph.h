#pragma once

#include "stdafx.h"
#include "Queue.h"

template<typename T>
class Graph
{
public:
	struct Edge;
	struct Node;

public:
	void AddNode(Node* node)
	{
		Node* nodeList = nodes;
		if (nodeList != nullptr)
		{
			while (nodeList->Next != nullptr)
				nodeList = nodeList->Next;

			nodeList->Next = node;
		}
		else
		{
			nodes = node;
		}
	}

	void AddEdge(Node* node, Edge* edge)
	{
		if (node->Edge != nullptr)
		{
			Edge* edgeList = node->Edge;

			while (edgeList->Next != nullptr)
				edgeList = edgeList->Next;

			edgeList->Next = edge;
		}
		else
		{
			node->Edge = edge;
		}
	}

	void Print()
	{
		Node* node = nullptr;
		Edge* edge = nullptr;

		if ((node = nodes) == nullptr)
			return;

		while (node != nullptr)
		{
			cout << node->Data << " : ";

			if ((edge = node->Edge) == nullptr)
			{
				node = node->Next;
				cout << endl;

				continue;
			}

			while (edge != nullptr)
			{
				cout << "->" << edge->Target->Data << " ";
				edge = edge->Next;
			}

			cout << endl;

			node = node->Next;
		}

		cout << endl;
			
	}

	void DFS(Node* node)
	{
		cout << node->Data << " -> ";

		node->bVisited = true;

		Edge* edge = node->Edge;
		while (edge != nullptr)
		{
			if (edge->Target != nullptr && edge->Target->bVisited == false)
				DFS(edge->Target);

			edge = edge->Next;
		}

	}

	void BFS(Node* node)
	{
		Edge* edge = nullptr;

		cout << node->Data << " -> ";

		node->bVisited = true; //�� ��带 �Ծ��ٰ� ǥ��

		queue.Push(queue.CreateNode(node)); //�׷���::��� -> ť::���� ��ȯ -> ť�� ��ť

		while (queue.IsEmpty() == false)
		{
			Queue<Node*>::Node* poped = queue.Pop(); //�׷��� ��带 ����
			node = poped->Data; //ť::��� �ȿ� �ִ� �׷���::��带 ����
			edge = node->Edge; //�׷���::��忡 ����� ���� �����ּҸ� ����

			//���� �ϳ��� ������
			while (edge != nullptr)
			{
				//���� ��� ������
				node = edge->Target;

				//Ÿ�� ��尡 �����ϰ� && �湮�� ���� ���� ��常
				if (node != nullptr && node->bVisited == false)
				{
					cout << node->Data << " -> ";

					node->bVisited = true;
					queue.Push(queue.CreateNode(node)); //��ť
				}

				edge = edge->Next;
			}
		}
	}

public:
	static Node* CreateNode(T data)
	{
		Node* node = new Node();
		node->Data = data;

		return node;
	}

	static Edge* CreateEdge(Node* start, Node* target, float weight = 0.f)
	{
		Edge* edge = new Edge();
		edge->Weight = weight;
		edge->Start = start;
		edge->Target = target;
		edge->Next = nullptr;

		return edge;
	}

	static void DestroyNode(Node* node)
	{
		while (node->Edge != nullptr)
		{
			Edge* edge = node->Edge->Next;
			DestroyEdge(edge);

			node->Edge = edge;
		}
	}

	void DestroyEdge(Edge* edge)
	{
		SafeDelete(edge);
	}

public:
	struct Edge
	{
		float Weight = 0.f;
		Node* Start = nullptr;
		Node* Target = nullptr;
		Edge* Next = nullptr;
	};

	struct Node
	{
		T Data;
		bool bVisited = false;
		int Index = -1;

		Node* Next = nullptr;
		Edge* Edge = nullptr;
	};

private:
	Node* nodes = nullptr;
	int count = 0;

	Queue<Node*> queue;
};