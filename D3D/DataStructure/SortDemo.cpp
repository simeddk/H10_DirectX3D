#include "stdafx.h"
#include "SortDemo.h"
#include "BTree.h"

void SortDemo::Initialize()
{
	int arr[5] = { 5, 1, 3, 2, 4 };

	cout << "정렬 전 : ";
	for (int i = 0; i < 5; i++)
		cout << arr[i] << ", ";
	cout << endl;

	//Bubble Sort
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (arr[j] > arr[j + 1])
			{
				int temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}

	cout << "정렬 후 : ";
	for (int i = 0; i < 5; i++)
		cout << arr[i] << ", ";
	cout << endl;

	//Shuffle
	srand((UINT)time(nullptr));
	for (int i = 0; i < 1000; i++)
	{
		int r1 = rand() % 5;
		int r2 = rand() % 5;

		int temp = arr[r1];
		arr[r1] = arr[r2];
		arr[r2] = temp;
	}

	cout << "셔플 후 : ";
	for (int i = 0; i < 5; i++)
		cout << arr[i] << ", ";
	cout << endl;

	//Selection Sort
	for (int i = 0; i < 5; i++)
	{
		for (int j = i; j < 5; j++)
		{
			if (arr[i] > arr[j])
			{
				swap(arr[i], arr[j]);
			}
		}
	}

	cout << "정렬 후 : ";
	for (int i = 0; i < 5; i++)
		cout << arr[i] << ", ";
	cout << endl;

}
