#include "stdafx.h"
#include "PriorityQueueDemo.h"
#include "PriorityQueue.h"

void PrintQueue(PriorityQueue<std::string>& queue, PriorityQueue<std::string>::Node* nodes)
{
	for (int i = 0; i < queue.Size(); i++)
		printf("우선순위 : %d, 할일 : %s\n", nodes[i].Priority, nodes[i].Data.c_str());
}

void PriorityQueueDemo::Initialize()
{
	PriorityQueue<string> queue(6);
	queue.Enqueue(PriorityQueue<string>::Node(66, "VIP 고객 클레임"));
	queue.Enqueue(PriorityQueue<string>::Node(34, "결재 불가"));
	queue.Enqueue(PriorityQueue<string>::Node(12, "서버 접속 불가"));
	queue.Enqueue(PriorityQueue<string>::Node(45, "개인 정보 노출"));
	queue.Enqueue(PriorityQueue<string>::Node(35, "DB 장애"));
	queue.Enqueue(PriorityQueue<string>::Node(87, "환불 클레임"));

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


