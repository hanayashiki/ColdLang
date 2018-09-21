#include "stdafx.h"
#include "Variable.h"

namespace IR::OperandType {

	Variable IR::Variable::AccVal(Word::mock(L"Acc"), true);

	Variable::Variable(shared_ptr<Token> && token, bool is_static)
		: Symbol(std::move(token), is_static)
	{

	}

	Variable::Variable(shared_ptr<Token> & token, bool is_static)
		: Symbol(std::move(token), is_static)
	{

	}

	Variable::Variable(shared_ptr<Token> & token, ValueType const_type, bool is_static)
		: Symbol(std::move(token), is_static), type_(const_type), const_type_(true)
	{
	}

	Variable::Variable(shared_ptr<Token> && token, ValueType const_type, bool is_static)
		: Symbol(std::move(token), is_static), type_(const_type), const_type_(true)
	{
	}

	const wchar_t * Variable::get_name() const
	{
		return static_cast<Word*>(get_token().get())->get_word();
	}

	bool Variable::is_const_type() const
	{
		return const_type_;
	}

	Variable::ValueType Variable::get_type() const
	{
		return type_;
	}

}