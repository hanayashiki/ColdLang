#include "stdafx.h"

namespace IR {

	Symbol* Symbol::Acc = &Variable::AccVal;

	OperandType::Symbol::Symbol(shared_ptr<Token> && token, bool is_static, LRValueType vt)
		: token_(token), value_type_(vt),
		is_temp_(false), nest_level_(0), offset_(0),
		is_static_(is_static)
	{
	}

	const shared_ptr<Token> & OperandType::Symbol::get_token() const
	{
		return token_;
	}

	size_t OperandType::Symbol::get_id()
	{
		return id_;
	}

	void OperandType::Symbol::set_value_type(LRValueType vt)
	{
		this->value_type_ = vt;
	}

	OperandType::Symbol::LRValueType OperandType::Symbol::get_value_type()
	{
		return this->value_type_;
	}

	void OperandType::Symbol::set_id(size_t id)
	{
		id_ = id;
	}
	wstring OperandType::Symbol::to_string() const
	{
		return token_->to_string();
	}
	void OperandType::Symbol::set_temp(bool is_temp)
	{
		is_temp_ = is_temp;
	}
	bool OperandType::Symbol::is_temp()
	{
		return is_temp_;
	}
	bool OperandType::Symbol::is_static()
	{
		return is_static_;
	}
	void OperandType::Symbol::set_nest_level(uint32_t nest_level)
	{
		nest_level_ = nest_level;
	}
	uint32_t OperandType::Symbol::get_nest_level()
	{
		return nest_level_;
	}
	OperandType::Symbol::~Symbol()
	{
	}
}
