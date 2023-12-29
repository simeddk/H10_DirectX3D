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

	//UnresolvedMergeConflict. Search, Remove ÇÒ Â÷·ÊÀÓ
}
