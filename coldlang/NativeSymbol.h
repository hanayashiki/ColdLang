#pragma once
#include "stdafx.h"
#include "Symbol.h"

namespace IR 
{
	namespace OperandType 
	{
		class NativeSymbol : public Symbol
		{
		public:
			NativeSymbol(const wchar_t * name) :
				Symbol(Word::mock(name), true)
			{
			}

			const wchar_t * get_name()
			{
				return get_token().get()->get_raw_string();
			}
		};
	}
}
