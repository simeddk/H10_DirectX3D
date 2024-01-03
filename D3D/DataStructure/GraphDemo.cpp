#include "stdafx.h"
#include "GraphDemo.h"
#include "Graph.h"

void GraphDemo::Initialize()
{
	Graph<char> graph;
	Graph<char>::Node* A = Graph<char>::CreateNode('A');
	Graph<char>::Node* B = Graph<char>::CreateNode('B');
	Graph<char>::Node* C = Graph<char>::CreateNode('C');
	Graph<char>::Node* D = Graph<char>::CreateNode('D');
	Graph<char>::Node* E = Graph<char>::CreateNode('E');

	graph.AddNode(A);
	graph.AddNode(B);
	graph.AddNode(C);
	graph.AddNode(D);
	graph.AddNode(E);

	graph.AddEdge(A, Graph<char>::CreateEdge(A, B)); //A -> B
	graph.AddEdge(A, Graph<char>::CreateEdge(A, C)); //A -> C
	graph.AddEdge(A, Graph<char>::CreateEdge(A, D)); //A -> D
	graph.AddEdge(A, Graph<char>::CreateEdge(A, E)); //A -> E

	graph.AddEdge(B, Graph<char>::CreateEdge(B, A)); // B -> A
	graph.AddEdge(B, Graph<char>::CreateEdge(B, C)); // B -> C
	graph.AddEdge(B, Graph<char>::CreateEdge(B, E)); // B -> E

	graph.AddEdge(C, Graph<char>::CreateEdge(C, A)); //C -> A
	graph.AddEdge(C, Graph<char>::CreateEdge(C, B)); //C -> B

	graph.AddEdge(D, Graph<char>::CreateEdge(D, A)); //D -> A
	graph.AddEdge(D, Graph<char>::CreateEdge(D, E)); //D -> E

	graph.AddEdge(E, Graph<char>::CreateEdge(E, A)); //E -> A
	graph.AddEdge(E, Graph<char>::CreateEdge(E, B)); //E -> B
	graph.AddEdge(E, Graph<char>::CreateEdge(E, D)); //E -> D

	graph.Print();

	printf("DFS : ");
	graph.DFS(A);
	printf("\n");

	A->bVisited = B->bVisited = C->bVisited = D->bVisited = E->bVisited = false;
	printf("BFS : ");
	graph.BFS(D);
}


