#pragma once

#include "stdafx.h"

template<typename T>
class PriorityQueue
{
public:
	struct Node;

public:
	PriorityQueue(int initSize)
		: capacity(initSize)
		, size(0)
	{
		nodes = new Node[capacity];
	}

	~PriorityQueue()
	{
		SafeDeleteArray(nodes);
	}

	int Parent(int index)
	{
		return (int)((index - 1) / 2);
	}

	int Left(int index)
	{
		return index * 2 + 1;
	}

	void SwapNode(int index1, int index2)
	{
		Node* temp = new Node(); //malloc

		memcpy(temp, &nodes[index1], sizeof(Node));
		memcpy(&nodes[index1], &nodes[index2], sizeof(Node));
		memcpy(&nodes[index2], temp, sizeof(Node));
	}

	void Enqueue(Node newNode)
	{
		int curr = size;

		//Capacity�� full�� �������� ��
		if (size == capacity)
		{
			Node* newNodes = new Node[capacity * 2];

			memcpy(newNodes, nodes, sizeof(Node) * capacity);
			
			SafeDeleteArray(nodes);
			nodes = newNodes;

			capacity *= 2;
		}

		nodes[curr] = newNode;

		//����
		int parent = Parent(curr);
		while (curr > 0 && nodes[curr].Priority < nodes[parent].Priority)
		{
			SwapNode(curr, parent);

			curr = parent;
			parent = Parent(curr);
		}

		size++;
	}

	Node Dequeue()
	{
		int parent = 0;
		int left = 0;
		int right = 0;

		Node result = nodes[0];

		//�ּҰ� ��带 ���� ������ ��ġ�� �̵�
		size--;
		SwapNode(0, size);
		
		left = Left(0);
		right = left + 1;

		//����
		while (true)
		{
			int selected = 0;

			//Left, Right�� �迭�� ������ ��� ���
			if (left >= size)
				break;

			if (right >= size)
				selected = left;
			else
			{
				if (nodes[left].Priority > nodes[right].Priority)
					selected = right;
				else
					selected = left;
			}
		
			if (nodes[selected].Priority < nodes[parent].Priority)
			{
				SwapNode(selected, parent);

				parent = selected;
			}
			else
				break;

			left = Left(parent);
			right = left + 1;
		}

		return result;
	}

	void Print()
	{
		for (int i = 0; i < size; i++)
		{
			printf("%d, ", nodes[i].Data);
		}
		printf("\n");
	}

	Node* Nodes() { return nodes; }
	int Size() { return size; }

private:
	struct Node
	{
		T Data;
		int Priority;

		Node() = default;

		Node(int priority, T data) 
			: Priority(priority),  Data(data) 
		{}
	};

private:
	Node* nodes;

	int capacity;		//�Ҵ� ���� ũ��
	int size;			//���� ��� ũ��
};