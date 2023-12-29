#include "stdafx.h"
#include "BSearchDemo.h"
#include "BTree.h"


int Compare(const void* data1, const void* data2)
{
	DataDesc* struct1 = (DataDesc*)data1;
	DataDesc* struct2 = (DataDesc*)data2;

	if (struct1->Data > struct2->Data)
		return 1;

	else if (struct1->Data < struct2->Data)
		return -1;

	else
		return 0;
}

void BSearchDemo::Initialize()
{
	ReadFile("../../_Datas/SortedData.txt");

	int target = 32763;

	printf("Sequence Search\n");
	printf("찾은 값 : %d\n", SequenceSearch(target, count));
	printf("%d번만에 찾음\n", count);

	printf("\n\nBinary Search\n");
	printf("찾은 값 : %d\n", BinarySearch(target, count));
	printf("%d번만에 찾음\n", count);

	//void* value = bsearch((void*)&target, datas, USHRT_MAX, sizeof(DataDesc), Compare);
	//printf("찾은 값 : %d\n", (UINT*)value);
	//int a = 0;
}

void BSearchDemo::ReadFile(string path)
{
	FILE* buffer;
	fopen_s(&buffer, path.c_str(), "r");
	{
		for (UINT i = 0; i < USHRT_MAX; i++)
			fscanf_s(buffer, "%d,%d", &datas[i].Index, &datas[i].Data);
	}
	fclose(buffer);
}

UINT BSearchDemo::SequenceSearch(UINT target, int& count)
{
	for (UINT i = 0; i < USHRT_MAX; i++)
	{
		if (datas[i].Data == target)
		{
			count = i;
			return datas[i].Data;
		}
	}

	return 0;
}

UINT BSearchDemo::BinarySearch(UINT target, int& count)
{
	count = 0;

	int left = 0;
	int right = USHRT_MAX - 1;

	while (left <= right)
	{
		int middle = (left + right) / 2;

		if (datas[middle].Data == target)
			return datas[middle].Data;
		else if (datas[middle].Data < target)
			left = middle + 1;
		else if (datas[middle].Data > target)
			right = middle - 1;

		count++;
	}

	return 0;
}

