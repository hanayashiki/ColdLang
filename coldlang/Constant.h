#pragma once
#include "stdafx.h"
#include "RuntimeObject.h"

namespace IR
{
	namespace OperandType
	{
		class Constant : public Symbol
		{
		private:
			Runtime::RuntimeValue * value_;
		public:
			Constant(shared_ptr<Token> & token, Runtime::RuntimeValue * value)
				:
				value_(value), Symbol(std::move(token))
			{
			}
			Runtime::RuntimeValue * get_value()
			{
				return value_;
			}
			wstring to_string() override
			{
				return wstring(get_token()->get_raw_string())
					+ L"@" + to_wstring(get_token()->get_line_index()) + wstring(L",") + to_wstring(get_token()->get_column_index());
			}
			~Constant()
			{
				delete value_;
			}
		};
	}
}