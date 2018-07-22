#pragma once
#include "stdafx.h"
#include "Symbol.h"

namespace IR
{
	namespace OperandType
	{
		class Variable : public Symbol
		{
		public:
			Variable(shared_ptr<Token> & token);
			Variable(shared_ptr<Token> && token);
			const wchar_t * get_name();
		};
	}
}
