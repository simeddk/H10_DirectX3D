#include "stdafx.h"
#include "BinarySearchTreeDemo.h"
#include "BinarySearchTree.h"

#define Tree BinarySearchTree<int>

void BinarySearchTreeDemo::Initialize()
{
	Tree tree;
	auto root = Tree::CreateNode(123);
	tree.Root(root);

	tree.Insert(&root, Tree::CreateNode(22));
	tree.Insert(&root, Tree::CreateNode(999));
	tree.Insert(&root, Tree::CreateNode(424));
	tree.Insert(&root, Tree::CreateNode(17));
	tree.Insert(&root, Tree::CreateNode(3));
	tree.Insert(&root, Tree::CreateNode(98));
	tree.Insert(&root, Tree::CreateNode(34));
	tree.Insert(&root, Tree::CreateNode(759));
	tree.Insert(&root, Tree::CreateNode(320));
	tree.Insert(&root, Tree::CreateNode(1));

	tree.InOrder(root);

	int count = 0;
	Tree::Node* result = tree.Search(root, 999, count);
	cout << endl << endl;
	cout << count << "번 만에 찾음, 찾은 값 : " << result->Data << endl;

	cout << endl << endl;
	result = tree.SearchMin(result);
	cout << "이 그룹의 최소값은 " << result->Data << endl;
	
	Tree::Node* node = nullptr;
	node = tree.Remove(root, nullptr, 22);
	Tree::DestroyNode(node);

	cout << endl << endl;
	tree.InOrder(root);

	tree.Insert(&root, Tree::CreateNode(19));
	cout << endl << endl;
	tree.InOrder(root);

	tree.Insert(&root, Tree::CreateNode(18));
	cout << endl << endl;
	tree.InOrder(root);

	node = tree.Remove(root, nullptr, 17);
	Tree::DestroyNode(node);
	cout << endl << endl;
	tree.InOrder(root);
}
