#pragma once
#include "stdafx.h"

class TokenException : public std::exception
{
private:
	Token * token_;
public:
	TokenException(const char* message, Token* token) : std::exception(message)
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

class NumberFormatException : public TokenException
{
public:
	NumberFormatException(const char* message, Token* token) : TokenException(message, token)
	{
	}
};