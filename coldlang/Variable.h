#pragma once
#include "stdafx.h"

namespace IR
{
	namespace OperandType
	{
		class Variable : public Symbol
		{
		private:
			shared_ptr<Token> token_;
		public:
			Variable(shared_ptr<Token> && token);
			const wchar_t * get_name();
		};
	}
}