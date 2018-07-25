#pragma once
#include "stdafx.h"
#include "RuntimeObject.h"

namespace IR
{
	using namespace OperandType;
	class ConstantTable
	{
	private:
		size_t id_;
		vector<Runtime::RuntimeObject*> constants_;
	public:
		ConstantTable();
		size_t add(Runtime::RuntimeObject* constant);
		~ConstantTable();
	};
}

