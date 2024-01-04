#include "stdafx.h"
#include "HashDemo.h"
#include "Chaining.h"

int MakeIndexFromString(string key, int tableSize)
{
	int index = 0;

	for (int i = 0; i < key.size(); i++)
		index = (index << 3) + key[i]; //�ڸ��� ����

	return index % tableSize; //��������
}


void HashDemo::Initialize()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x06);

	string key = "abcdefghij";
	int index = MakeIndexFromString(key, 12289);
	cout << index << endl;

	key = "BA";
	cout << MakeIndexFromString(key, 12289) << endl;

	key = "AI";
	cout << MakeIndexFromString(key, 12289) << endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x04);
	cout << " -> Chaining" << endl;

	Chaining<int> hash(12289);
	hash.Set("AI", 2000);
	hash.Set("JA", 1999);
	hash.Set("BA", 2001);
	hash.Set("EE", 2002);
	
	printf("AI �ε����� ���� : %d\n", hash.Get("AI")); // -> 2000
	printf("JA �ε����� ���� : %d\n", hash.Get("JA")); // -> 1999
	printf("BA �ε����� ���� : %d\n", hash.Get("BA")); // -> 2001
	printf("EE �ε����� ���� : %d\n", hash.Get("EE")); // -> 2002
}


