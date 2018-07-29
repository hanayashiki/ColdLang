#pragma once
#include "stdafx.h"
#include "Function.h"

namespace IR
{
	class FunctionTable
	{
	private:
		size_t id_;
		vector<Function*> functions_;
	public:
		FunctionTable();
		size_t add(Function* function);
		void compile_all();
		~FunctionTable();
	};
}