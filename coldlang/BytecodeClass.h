#pragma once
#include "stdafx.h"

namespace IR
{
	namespace BytecodeClass
	{
		class LoadAttributeToAcc : public VirtualLoadAttributeToAcc
		{
		public:
			void init(OperandType::Symbol*, OperandType::Index*) override
			{
				wcout << "fuck you this is not implemented" << endl;
			}
			void dump_byte(char buf[]) override
			{
				
			}
		};

		class LoadSymbolToAcc : public VirtualLoadSymbolToAcc
		{
		private:
			OperandType::Symbol * target_;
		public:
			void init(OperandType::Symbol* t) override
			{
				target_ = t;
			}
			void dump_byte(char buf[]) override
			{
				
			}
			wstring to_string() override
			{
				return tutils::ascii_string_to_wstring(get_name())
					+ L" " + target_->to_string();
			}
		};

		class CallFunc : public VirtualCallFunc
		{
		public:
			void init(OperandType::Index*) override
			{
				wcout << "fuck you this is not implemented" << endl;
			}
			void dump_byte(char buf[]) override
			{

			}
		};

		class Inc : public VirtualInc
		{
		private:
			OperandType::Symbol * target_;
		public:
			void init(OperandType::Symbol* s) override
			{
				target_ = s;
			}
			void dump_byte(char buf[]) override
			{

			}
			wstring to_string() override
			{
				return tutils::ascii_string_to_wstring(get_name())
				+ L" " + target_->to_string();
			}
		};

		class Decre : public VirtualDecre
		{
		private:
			OperandType::Symbol * target_;
		public:
			void init(OperandType::Symbol* s) override
			{
				target_ = s;
			}
			void dump_byte(char buf[]) override
			{

			}
			wstring to_string() override
			{
				return tutils::ascii_string_to_wstring(get_name())
					+ L" " + target_->to_string();
			}
		};
	}
}
