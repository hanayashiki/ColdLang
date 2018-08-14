#include "stdafx.h"
#include "ConstantTable.h"

namespace IR
{
	ConstantTable::ConstantTable()
		: id_(0)
	{
	}

	size_t ConstantTable::add(CldRuntime::RuntimeObject* constant)
	{
		constants_.push_back(constant);
		return id_++;
	}

	ConstantTable::~ConstantTable()
	{
		for (const auto constant : constants_)
		{
			delete constant;
		}
	}
}