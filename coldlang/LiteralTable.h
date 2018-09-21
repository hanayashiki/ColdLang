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
		Literal * true_constant_;
		Literal * false_constant_;
		Literal * none_constant_;
	public:
		LiteralTable();
		Literal * add_native_constant(const wchar_t * name, CldRuntime::RuntimeValue *);
		size_t add(Literal* literal);
		Literal * get_true_constant();
		Literal * get_false_constant();
		Literal * get_none_constant();
		~LiteralTable();
	};
}

