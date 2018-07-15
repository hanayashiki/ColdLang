#pragma once
#include "stdafx.h"

namespace IR
{
	namespace OperandType
	{
		class Symbol
		{
		public:
			enum ValueType
			{
				LValue = 1,
				RValue
			};
		private:
			shared_ptr<Token> token_;
			size_t id_;
			ValueType value_type_;
		public:
			Symbol(shared_ptr<Token> && token_, ValueType vt = LValue);
			shared_ptr<Token> & get_token();
			size_t get_id();
			void set_value_type(ValueType);
			ValueType get_value_type();
			void set_id(size_t);
			virtual wstring to_string();
		};
	}
}