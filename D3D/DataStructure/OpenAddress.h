#pragma once

#include "stdafx.h"

template<typename T>
class OpenAddress
{
public:
	struct Element;

public:
	OpenAddress(int size)
		: tableSize(size)
		, occupiedCount(0)
	{
		table = new Element[tableSize];
		memset(table, 0, sizeof(Element) * tableSize);
	}

	~OpenAddress()
	{
		SafeDeleteArray(table);
	}

	int Hash(char* key)
	{
		int index = 0;

		for (int i = 0; i < strlen(key); i++)
			index = (index << 3) + key[i];

		return index % tableSize;
	}

	int Hash2(char* key)
	{
		int index = 0;

		for (int i = 0; i < strlen(key); i++)
			index = (index << 3) + key[i];

		return index % (tableSize - 3);
	}

	void ExtendTable()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x04);
		printf("70%% �̻� ����. ���̺� ����� Ȯ���մϴ�.\n");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);

		int oldSize = tableSize;
		tableSize *= 2;

		Element* oldTable = table;
		table = new Element[tableSize];

		for (int i = 0; i < oldSize; i++)
		{
			if (oldTable[i].bOccupied == true)
				Set(oldTable[i].Key, oldTable[i].Value);
		}

		SafeDeleteArray(oldTable);
	}


	void Set(char* key, T value)
	{
		//��뷮 üũ
		float usage = (float)occupiedCount / (float)tableSize;
		if (usage > 0.7f)
			ExtendTable();

		int index = Hash(key);
		int spare = Hash2(key);

		//�浹�� ���
		while (table[index].bOccupied == true && strcmp(table[index].Key, key) != 0)
		{
			printf("[!] %s Key ���� �� [%d]�� �̹� %s Key�� �����մϴ�.", key, index, table[index].Key);
			index = (index + spare) % tableSize;
			printf(" -> �ٸ� �ε��� [%d]�� �̵��մϴ�.\n", index);
		}

		table[index].Key = new char[strlen(key) + 1];
		
		strcpy_s(table[index].Key, sizeof(key) * strlen(key + 1), key);
		table[index].Value = value;
		table[index].bOccupied = true;

		occupiedCount++;

		printf("Key(%s)�� [%d] �ε����� �����\n", key, index);
	}

	T Get(char* key, int keyLength)
	{
		int index = Hash(key);
		int spare = Hash2(key);

		while (table[index].bOccupied == true && strcmp(table[index].Key, key) != 0)
			index = (index + spare) % tableSize;

		return table[index].Value;
	}

public:
	struct Element
	{
		char* Key;
		T Value;

		bool bOccupied;
	};

private:
	Element* table;

	int tableSize;
	int occupiedCount;
};