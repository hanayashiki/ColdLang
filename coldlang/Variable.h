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
			static Variable AccVal;

			Variable(shared_ptr<Token> & token);
			Variable(shared_ptr<Token> && token);
			const wchar_t * get_name() const;
			void set_var_id(size_t var_id) { this->var_id = var_id; }
			size_t get_var_id() const { return var_id; }
		private:
			size_t var_id;
		};
	}
}
