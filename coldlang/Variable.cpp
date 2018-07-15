#include "stdafx.h"

IR::OperandType::Variable::Variable(shared_ptr<Token> && token)
	: Symbol(std::move(token))
{

}

const wchar_t * IR::OperandType::Variable::get_name()
{
	return static_cast<Word*>(get_token().get())->get_word();
}
