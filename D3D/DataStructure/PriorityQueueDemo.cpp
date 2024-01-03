#include "stdafx.h"
#include "PriorityQueueDemo.h"
#include "PriorityQueue.h"

void PrintQueue(PriorityQueue<std::string>& queue, PriorityQueue<std::string>::Node* nodes)
{
	for (int i = 0; i < queue.Size(); i++)
		printf("�켱���� : %d, ���� : %s\n", nodes[i].Priority, nodes[i].Data.c_str());
}

void PriorityQueueDemo::Initialize()
{
	PriorityQueue<string> queue(6);
	queue.Enqueue(PriorityQueue<string>::Node(66, "VIP �� Ŭ����"));
	queue.Enqueue(PriorityQueue<string>::Node(34, "���� �Ұ�"));
	queue.Enqueue(PriorityQueue<string>::Node(12, "���� ���� �Ұ�"));
	queue.Enqueue(PriorityQueue<string>::Node(45, "���� ���� ����"));
	queue.Enqueue(PriorityQueue<string>::Node(35, "DB ���"));
	queue.Enqueue(PriorityQueue<string>::Node(87, "ȯ�� Ŭ����"));

	PriorityQueue<string>::Node* nodes = queue.Nodes();
	PrintQueue(queue, nodes);

	PriorityQueue<string>::Node node = queue.Dequeue();
	printf("\n\n -> %d, %s\n", node.Priority, node.Data.c_str());
	PrintQueue(queue, nodes);

	node = queue.Dequeue();
	printf("\n\n -> %d, %s\n", node.Priority, node.Data.c_str());
	PrintQueue(queue, nodes);

	node = queue.Dequeue();
	printf("\n\n -> %d, %s\n", node.Priority, node.Data.c_str());
	PrintQueue(queue, nodes);
}


