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
			uint32_t nest_level_;
			uint32_t offset_;
		public:
			Symbol(shared_ptr<Token> && token_, LRValueType vt = LValue);
			virtual ~Symbol();

			const shared_ptr<Token> & get_token() const;
			size_t get_id();
			void set_value_type(LRValueType);
			LRValueType get_value_type();
			void set_id(size_t);
			void set_temp(bool is_temp);
			bool is_temp();
			void set_nest_level(uint32_t);
			uint32_t get_nest_level();
			virtual wstring to_string() const;
		};

		static inline wstring to_string(const Symbol* symbol)
		{
			return symbol->to_string();
		}
	}
}
