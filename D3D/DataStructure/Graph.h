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

		node->bVisited = true; //이 노드를 왔었다고 표시

		queue.Push(queue.CreateNode(node)); //그래프::노드 -> 큐::노드로 변환 -> 큐에 인큐

		while (queue.IsEmpty() == false)
		{
			Queue<Node*>::Node* poped = queue.Pop(); //그래프 노드를 꺼냄
			node = poped->Data; //큐::노드 안에 있는 그래프::노드를 꺼냄
			edge = node->Edge; //그래프::노드에 연결된 간선 시작주소를 꺼냄

			//간선 하나씩 꺼내기
			while (edge != nullptr)
			{
				//인접 노드 꺼내기
				node = edge->Target;

				//타겟 노드가 존재하고 && 방문한 적이 없는 노드만
				if (node != nullptr && node->bVisited == false)
				{
					cout << node->Data << " -> ";

					node->bVisited = true;
					queue.Push(queue.CreateNode(node)); //인큐
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