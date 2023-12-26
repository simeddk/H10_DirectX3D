#include "stdafx.h"
#include "QueueDemo.h"
#include "Queue.h"

void QueueDemo::Initialize()
{
	Queue<int> queue;

	queue.Push(Queue<int>::CreateNode(10));
	queue.Push(Queue<int>::CreateNode(1));
	queue.Push(Queue<int>::CreateNode(20));
	queue.Push(Queue<int>::CreateNode(30));
	queue.Push(Queue<int>::CreateNode(40));
	queue.Push(Queue<int>::CreateNode(70));

	printf("Count : %d\n", queue.Count());

	while (queue.IsEmpty() == false)
	{
		Queue<int>::Node* poped = queue.Pop();
		printf("Pop : %d\n", poped->Data);

		Queue<int>::DestroyNode(poped);
	}
	
}
