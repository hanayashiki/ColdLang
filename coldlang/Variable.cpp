#include "stdafx.h"

IR::Variable IR::Variable::AccVal(Word::mock(L"Acc"));

IR::OperandType::Variable::Variable(shared_ptr<Token> && token)
	: Symbol(std::move(token))
{

}

IR::OperandType::Variable::Variable(shared_ptr<Token> & token)
	: Symbol(std::move(token))
{

}


const wchar_t * IR::OperandType::Variable::get_name() const
{
	return static_cast<Word*>(get_token().get())->get_word();
}
