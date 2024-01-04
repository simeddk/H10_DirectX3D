#pragma once

#include "stdafx.h"

template<typename ValueType>
class Chaining
{
public:
	struct Node;
	typedef Node* List;

public:
	Chaining(int size)
		: size(size)
	{
		table = new List[size];
		memset(table, 0, sizeof(List) * size);
	}

	~Chaining()
	{
		SafeDeleteArray(table);
	}

private:
	int Hash(const char* key)
	{
		int index = 0;

		for (int i = 0; i < strlen(key); i++)
			index = (index << 3) + key[i];

		return index % size;
	}

public:
	void Set(const char* key, ValueType value)
	{
		int index = Hash(key);

		Node* newNode = CreateNode(key, value);

		if (table[index] != nullptr)
		{
			List alreadyExist = table[index];
			newNode->Next = alreadyExist;
			table[index] = newNode;

			return;
		}

		table[index] = newNode;
	}

	ValueType Get(const char* key) //"BA"
	{
		int index = Hash(key);

		List hashList = table[index];
		List target = nullptr;

		//해당 Key에 대한 검색 결과가 없다면
		if (hashList == nullptr)
			return ValueType();

		while (true)
		{
			//원하는 키를 찾은 경우
			if (strcmp(hashList->Key, key) == 0)
			{
				target = hashList;
				break;
			}

			//몾 찾은 경우
			if (hashList->Next == nullptr)
				return ValueType();
			else
				hashList = hashList->Next;
		}


		return target->Value;
	}

public:
	static Node* CreateNode(const char* key, ValueType value)
	{
		Node* node = new Node(); //new -> delete, malloc -> free

		node->Key = (char*)malloc(sizeof(key) * strlen(key + 1)); //new char[strlen(key + 1)]
		strcpy_s(node->Key, sizeof(key), key);
		
		node->Value = value;
		node->Next = nullptr;

		return node;
	}

	static void DestroyNode(Node* node)
	{
		free(node->Key);

		SafeDelete(node);
	}


public:
	struct Node
	{
		char* Key;
		ValueType Value;

		Node* Next;
	};

	List* table;
	int size;
};