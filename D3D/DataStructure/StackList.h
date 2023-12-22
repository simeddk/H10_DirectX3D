#pragma once

namespace StackNode
{
	struct Node
	{
		Node* Next;
		string Data;
	};
}

class StackList
{
public:
	StackList() = default;
	~StackList();

	void Push(StackNode::Node* newNode);
	StackNode::Node* Pop();

	int Size();

	static StackNode::Node* CreateNode(string data);
	static void DestroyNode(StackNode::Node* node);

	StackNode::Node* GetList() { return list; }

private:
	StackNode::Node* list = nullptr;
	StackNode::Node* top = nullptr;
};