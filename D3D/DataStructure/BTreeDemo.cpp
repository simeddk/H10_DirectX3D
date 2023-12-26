#include "stdafx.h"
#include "BTreeDemo.h"
#include "BTree.h"

void BTreeDemo::Initialize()
{
	BTree<char>::Node* A = BTree<char>::CreateNode('A');
	BTree<char>::Node* B = BTree<char>::CreateNode('B');
	BTree<char>::Node* C = BTree<char>::CreateNode('C');
	BTree<char>::Node* D = BTree<char>::CreateNode('D');
	BTree<char>::Node* E = BTree<char>::CreateNode('E');
	BTree<char>::Node* F = BTree<char>::CreateNode('F');
	BTree<char>::Node* G = BTree<char>::CreateNode('G');

	{
		BTree<char> tree;
		tree.Root(A);

		A->Left = B;

		B->Left = C;
		B->Right = D;

		A->Right = E;

		E->Left = F;
		E->Right = G;

		cout << "PreOrder :";
		tree.PreOrder(A);
		cout << endl;

		cout << "InOrder :";
		tree.InOrder(A);
		cout << endl;

		cout << "PostOrder :";
		tree.PostOrder(A);
		cout << endl;

		int a = 0;
	}

	int a = 0;
}
