#pragma once
#include "stdafx.h"
#include "Literal.h"

namespace IR
{
	using namespace OperandType;
	class LiteralTable
	{
	private:
		size_t id_;
		vector<Literal*> literals_;
	public:
		LiteralTable();
		size_t add(Literal* literal);
		~LiteralTable();
	};
}

