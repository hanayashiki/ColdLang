#pragma once
#include "stdafx.h"

class UnexpectedCharacterException : std::exception
{
private:
	Token * token_;
public:
	UnexpectedCharacterException(Token* token)
	{
		token_ = token;
	}
	const char * what()
	{
		return "unexpected character";
	}
	const Token * get_token() const
	{
		return token_;
	}
};
