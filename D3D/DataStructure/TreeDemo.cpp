#include "stdafx.h"
#include "TreeDemo.h"
#include "Tree.h"

void TreeDemo::Initialize()
{
	Tree<char> tree;

	Tree<char>::Node* A = Tree<char>::CreateNode('A');
	Tree<char>::Node* B = Tree<char>::CreateNode('B');
	Tree<char>::Node* C = Tree<char>::CreateNode('C');
	Tree<char>::Node* D = Tree<char>::CreateNode('D');
	Tree<char>::Node* E = Tree<char>::CreateNode('E');
	Tree<char>::Node* F = Tree<char>::CreateNode('F');
	Tree<char>::Node* G = Tree<char>::CreateNode('G');
	Tree<char>::Node* H = Tree<char>::CreateNode('H');
	Tree<char>::Node* I = Tree<char>::CreateNode('I');
	Tree<char>::Node* J = Tree<char>::CreateNode('J');
	Tree<char>::Node* K = Tree<char>::CreateNode('K');

	tree.Add(A, B);
	tree.Add(B, C);
	tree.Add(B, D);
	tree.Add(D, E);
	tree.Add(D, F);
	tree.Add(A, G);
	tree.Add(G, H);
	tree.Add(A, I);
	tree.Add(I, J);
	tree.Add(J, K);

	tree.Print(A, 0);//Pause
	//->Continue
}
