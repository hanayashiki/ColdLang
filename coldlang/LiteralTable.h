#pragma once
#include "stdafx.h"
#include "Literal.h"

namespace IR
{
	using namespace OperandType;
	class LiteralTable
	{
	private:
		vector<Literal*> literals_;
		vector<Constant*> constants_;
		Constant * true_constant_;
		Constant * false_constant_;
		Constant * none_constant_;
	public:
		LiteralTable();
		Constant * add_native_constant(const wchar_t * name, Runtime::RuntimeValue *);
		size_t add(Literal* literal);
		size_t add(Constant* constant);
		Constant * get_true_constant();
		Constant * get_false_constant();
		Constant * get_none_constant();
		~LiteralTable();
	};
}

