#include "stdafx.h"
#include "LiteralTable.h"
#include "Literal.h"

namespace IR {
	using namespace OperandType;
	LiteralTable::LiteralTable()
	{
		true_constant_ = add_native_constant(L"true", new CldRuntime::BooleanValue(true));
		false_constant_ = add_native_constant(L"false", new CldRuntime::BooleanValue(false));
		none_constant_ = add_native_constant(L"none", new CldRuntime::NoneValue());
	}

	Literal * LiteralTable::add_native_constant(const wchar_t * name, CldRuntime::RuntimeValue* val)
	{
		auto mock_token = std::dynamic_pointer_cast<Token>(Word::mock(name));
		Literal * constant = new Literal(mock_token, val);
		add(constant);
		return constant;
	}

	size_t LiteralTable::add(Literal * literal)
	{
		int id = literals_.size();
		literal->set_id(id);
		literals_.push_back(literal);
		return id;
	}

	Literal * LiteralTable::get_true_constant()
	{
		return true_constant_;
	}

	Literal * LiteralTable::get_false_constant()
	{
		return false_constant_;
	}

	Literal * LiteralTable::get_none_constant()
	{
		return none_constant_;
	}

	LiteralTable::~LiteralTable()
	{
		for (auto literal : literals_)
		{
			delete literal;
		}
	}
}