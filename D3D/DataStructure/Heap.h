#pragma once

#include "stdafx.h"

template<typename T>
class Heap
{
public:
	struct Node;

public:
	Heap(int initSize)
		: capacity(initSize)
		, size(0)
	{
		nodes = new Node[capacity];
	}

	~Heap()
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

	void Insert(Node newNode)
	{
		int curr = size;

		//Capacity가 full에 도달했을 때
		if (size == capacity)
		{
			Node* newNodes = new Node[capacity * 2];

			memcpy(newNodes, nodes, sizeof(Node) * capacity);
			
			SafeDeleteArray(nodes);
			nodes = newNodes;

			capacity *= 2;
		}

		nodes[curr] = newNode;

		//경쟁
		int parent = Parent(curr);
		while (curr > 0 && nodes[curr].Data < nodes[parent].Data)
		{
			SwapNode(curr, parent);

			curr = parent;
			parent = Parent(curr);
		}

		size++;
	}

	Node Remove()
	{
		int parent = 0;
		int left = 0;
		int right = 0;

		Node result = nodes[0];

		//최소값 노드를 가장 마지막 위치로 이동
		size--;
		SwapNode(0, size);
		
		left = Left(0);
		right = left + 1;

		//경쟁
		while (true)
		{
			int selected = 0;

			//Left, Right가 배열의 범위를 벗어난 경우
			if (left >= size)
				break;

			if (right >= size)
				selected = left;
			else
			{
				if (nodes[left].Data > nodes[right].Data)
					selected = right;
				else
					selected = left;
			}
		
			if (nodes[selected].Data < nodes[parent].Data)
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

private:
	struct Node
	{
		T Data;

		Node() = default;
		Node(T data) : Data(data) {}
	};

private:
	Node* nodes;

	int capacity;		//할당 받은 크기
	int size;			//실제 사용 크기
};