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
		vector<CldRuntime::RuntimeObject*> constants_;
	public:
		ConstantTable();
		size_t add(CldRuntime::RuntimeObject* constant);
		~ConstantTable();
	};
}

