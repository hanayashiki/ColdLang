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

	Constant* LiteralTable::add_native_constant(const wchar_t * name, CldRuntime::RuntimeValue* val)
	{
		auto mock_token = std::dynamic_pointer_cast<Token>(Word::mock(name));
		Constant * constant = new Constant(mock_token, val);
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

	size_t LiteralTable::add(Constant * constant)
	{
		int id = constants_.size();
		constant->set_id(id);
		constants_.push_back(constant);
		return id;
	}

	Constant * LiteralTable::get_true_constant()
	{
		return true_constant_;
	}

	Constant * LiteralTable::get_false_constant()
	{
		return false_constant_;
	}

	Constant * LiteralTable::get_none_constant()
	{
		return none_constant_;
	}

	LiteralTable::~LiteralTable()
	{
		for (auto constant : constants_)
		{
			delete constant;
		}
		for (auto literal : literals_)
		{
			delete literal;
		}
	}
}