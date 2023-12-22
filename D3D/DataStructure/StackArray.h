#pragma once

#define MAX_STACK_SIZE 5

template<typename T>
class StackArray
{
public:
	StackArray()
	{
		memset(values, 0, sizeof(T) * MAX_STACK_SIZE);
	}

	void Push(T data)
	{
		assert(top + 1 < MAX_STACK_SIZE);

		values[++top] = data;
	}

	T Pop()
	{
		bool bCheck = IsEmpty();
		assert(bCheck == false);

		T value = values[top];
		top--;

		return value;
	}

	T Front() 
	{
		assert(top > -1);
		return  values[top]; 
	}

	T Back() 
	{
		assert(top > -1);
		return  values[0]; 
	}
	
	bool IsEmpty() { return top < 0; }

private:
	T values[MAX_STACK_SIZE];

	int top = -1;
};