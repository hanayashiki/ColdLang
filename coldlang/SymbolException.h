#pragma once
#include "stdafx.h"

namespace IR {
	class SymbolException : std::exception
	{
	private:
		shared_ptr<Token> _token;
	public:
		SymbolException(const char * message, shared_ptr<Token> && token)
			: _token(token), exception(message)
		{
		}
		SymbolException(const char * message, shared_ptr<Token> & token)
			: _token(token), exception(message)
		{
		}
	};
}