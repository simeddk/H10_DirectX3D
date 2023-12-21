#pragma once

#include "Systems/IExecute.h"

struct Node
{
	int Data;
	Node* Next;
};

class ListDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {};
	virtual void Render() override {};
	virtual void PostRender() override {};
	virtual void ResizeScreen() override {};

private:
	Node* CreateNode(int data);
	void ReleaseNode(Node* node);

	void Push(Node** head, Node* newNode);
	void Insert(Node* current, Node* newNode);
	void Remove(Node** head, Node* removeNode);

	int GetListCount(Node* head);
	Node* GetNodeByIndex(Node* head, int index);
};