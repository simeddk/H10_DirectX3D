#include "stdafx.h"
#include "HeapDemo.h"
#include "Heap.h"

void HeapDemo::Initialize()
{
	Heap<int> heap(3);

	heap.Insert(12);
	heap.Insert(87);
	heap.Insert(111);
	heap.Insert(34);
	heap.Insert(16);
	heap.Insert(75);

	heap.Print();

	heap.Remove();
	heap.Print();

	heap.Remove();
	heap.Print();

	heap.Remove();
	heap.Print();

	heap.Remove();
	heap.Print();
}
