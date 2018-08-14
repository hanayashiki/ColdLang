#pragma once
#include "stdafx.h"

namespace IR
{
	namespace OperandType
	{
		class Symbol
		{
		public:
			static Symbol * Acc;
			enum LRValueType
			{
				LValue = 1,
				RValue
			};
		private:
			shared_ptr<Token> token_;
			size_t id_;
			LRValueType value_type_;
			bool is_temp_;
		public:
			Symbol(shared_ptr<Token> && token_, LRValueType vt = LValue);
			shared_ptr<Token> & get_token();
			size_t get_id();
			void set_value_type(LRValueType);
			LRValueType get_value_type();
			void set_id(size_t);
			virtual wstring to_string();
			void set_temp(bool is_temp);
			bool is_temp();
			virtual ~Symbol();
		};

		static inline wstring to_string(Symbol* symbol)
		{
			return symbol->to_string();
		}
	}
}
