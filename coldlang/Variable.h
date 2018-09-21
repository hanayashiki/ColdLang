#pragma once
#include "stdafx.h"
#include "Symbol.h"
#include "RuntimeValue.h"

namespace IR
{
	namespace OperandType
	{
		class Variable : public Symbol
		{
		public:
			static Variable AccVal;
			typedef CldRuntime::ValueType ValueType;

			Variable(shared_ptr<Token> & token, bool is_static = false);
			Variable(shared_ptr<Token> && token, bool is_static = false);
			Variable(shared_ptr<Token> & token, ValueType const_type, bool is_static = true);
			Variable(shared_ptr<Token> && token, ValueType const_type, bool is_static = true);
			const wchar_t * get_name() const;
			void set_var_id(size_t var_id) { this->var_id = var_id; }
			size_t get_var_id() const { return var_id; }

			bool is_const_type() const;
			ValueType get_type() const;
		private:
			size_t var_id;
			bool const_type_ = false;
			ValueType type_ = CldRuntime::AnyVal;
		};
	}
}
