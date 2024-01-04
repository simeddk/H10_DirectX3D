#pragma once

#include "stdafx.h"

template<typename T>
class OpenAddress
{

public:
	struct Element
	{
		char* Key;
		T Value;

		bool Occupied;
	};

};