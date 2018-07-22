#include "stdafx.h"
#include "FunctionTable.h"
namespace IR
{
	using namespace OperandType;
	FunctionTable::FunctionTable()
		:id_(0)
	{
	}

	size_t FunctionTable::add(Function * function)
	{
		functions_.push_back(function);
		function->set_id(id_);
		return id_++;
	}

	FunctionTable::~FunctionTable()
	{
		for (auto func : functions_)
		{
			delete func;
		}
	}
}