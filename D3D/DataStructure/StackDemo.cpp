#include "stdafx.h"
#include "StackDemo.h"
#include "StackArray.h"
#include "StackList.h"

void StackDemo::Initialize()
{
#ifdef StackArray
	StackArray<int> stack;
	stack.Push(100);
	stack.Push(200);

	cout << "First Pop : " << stack.Pop() << endl;

	stack.Push(300);
	stack.Push(400);
	stack.Pop();
	
	stack.Push(500);

	while (stack.IsEmpty() == false)
		printf("Loop Pop :  %d\n", stack.Pop());
#endif


	StackList stack;
	stack.Push(StackList::CreateNode("ABC"));
	stack.Push(StackList::CreateNode("DEF"));
	stack.Push(StackList::CreateNode("GHI"));
	stack.Push(StackList::CreateNode("JKL"));

	int count = stack.Size();
	for (int i = 0; i < count; i++)
	{
		 StackNode::Node* poped = stack.Pop();
		 printf("Poped : %s\n", poped->Data.c_str());

		 StackList::DestroyNode(poped);
	}
}
