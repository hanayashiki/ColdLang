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

	void FunctionTable::compile_all()
	{
		for (int i = 0; i < functions_.size(); i++)
		{
			functions_.at(i)->compile_to_bytecode();
		}
	}

	FunctionTable::~FunctionTable()
	{
		for (auto func : functions_)
		{
			delete func;
		}
	}
}