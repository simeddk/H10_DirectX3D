#include "stdafx.h"
#include "ListDemo.h"

void ListDemo::Initialize()
{

	Node* list = nullptr;

	//Push
	for (int i = 0; i < 5; i++)
	{
	 	Node* node = CreateNode(i);
		Push(&list, node);
	}

	//Insert
	Node* newNode = nullptr;
	newNode = CreateNode(-1);

	Node* current = GetNodeByIndex(list, 2);
	Insert(current, newNode);

	//Remoe
	current = GetNodeByIndex(list, 3);
	Remove(&list, current);

	current = GetNodeByIndex(list, 4);
	Remove(&list, current);

	//Count, GetByIndex
	int size = GetListCount(list);
	for (int i = 0; i < GetListCount(list); i++)
		cout << GetNodeByIndex(list, i)->Data << endl;
}

void ListDemo::Destroy()
{
	
}

void ListDemo::Update()
{
	
}

Node* ListDemo::CreateNode(int data)
{
	Node* node = new Node();
	node->Data = data;
	node->Next = nullptr;

	return node;
}

void ListDemo::ReleaseNode(Node* node)
{
	SafeDelete(node);
}

void ListDemo::Push(Node** head, Node* newNode)
{
	if ((*head) == nullptr)
	{
		*head = new Node();
		(*head)->Data = newNode->Data;
	}
	else
	{
		Node* current = *head;

		while (current->Next != nullptr)
			current = current->Next;

		current->Next = newNode;
	}
}

void ListDemo::Insert(Node* current, Node* newNode)
{
	newNode->Next = current->Next;
	current->Next = newNode;
}

void ListDemo::Remove(Node** head, Node* removeNode)
{
	//[0] 노드를 삭제하고 싶은 경우
	if (*head == removeNode)
	{
		*head = removeNode->Next;
	}
	//중간에 있는 노드를 삭제하고 싶은 경우
	else
	{
		Node* current = *head;

		while (current != nullptr && current->Next != removeNode)
			current = current->Next;

		if (current != nullptr)
			current->Next = removeNode->Next;
	}
}

int ListDemo::GetListCount(Node* head)
{
	int count = 0;

	Node* current = head;

	while (current != nullptr)
	{
		current = current->Next;
		count++;
	}


	return count;
}

//index = 3 -> return list[3]
Node* ListDemo::GetNodeByIndex(Node* head, int index)
{
	Node* current = head;

	//tail까지 가는 조건 && 
	while (current->Next != nullptr && (--index) >= 0)
		current = current->Next;

	return current;
}
