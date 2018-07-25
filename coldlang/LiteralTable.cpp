#include "stdafx.h"
#include "LiteralTable.h"
#include "Literal.h"

namespace IR {
	using namespace OperandType;
	LiteralTable::LiteralTable()
	{
	}

	size_t LiteralTable::add(Literal * literal)
	{
		literals_.push_back(literal);
		literal->set_id(id_);
		return id_++;
	}

	LiteralTable::~LiteralTable()
	{
		for (auto literal : literals_)
		{
			delete literal;
		}
	}
}