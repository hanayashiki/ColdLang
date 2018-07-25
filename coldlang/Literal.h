#pragma once
#include "stdafx.h"
#include "RuntimeObject.h"

namespace IR
{
	namespace OperandType
	{
		class Literal : public Symbol
		{
		private:
			Runtime::RuntimeObject * object_;
		public:
			Literal(shared_ptr<Token> & token, Runtime::RuntimeObject * object)
				:
				object_(object), Symbol(std::move(token))
			{
			}
			Runtime::RuntimeObject * get_object()
			{
				return object_;
			}
			wstring to_string() override
			{
				return wstring(get_token()->get_raw_string())
				+ L"@" + to_wstring(get_token()->get_line_index()) + wstring(L",") +  to_wstring(get_token()->get_column_index());
			}
		};
	}
}
