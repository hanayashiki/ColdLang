#include "stdafx.h"

namespace IR {
	OperandType::Symbol::Symbol(shared_ptr<Token> && token, ValueType vt)
		: token_(token), value_type_(vt)
	{
	}

	shared_ptr<Token> & OperandType::Symbol::get_token()
	{
		return token_;
	}

	size_t OperandType::Symbol::get_id()
	{
		return id_;
	}

	void OperandType::Symbol::set_value_type(ValueType vt)
	{
		this->value_type_ = vt;
	}

	OperandType::Symbol::ValueType OperandType::Symbol::get_value_type()
	{
		return this->value_type_;
	}

	void OperandType::Symbol::set_id(size_t id)
	{
		id_ = id;
	}
	wstring OperandType::Symbol::to_string()
	{
		return token_->to_string();
	}
}