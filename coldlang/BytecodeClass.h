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

		class Mul : public VirtualMul
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

		class MulTwo : public VirtualMulTwo
		{
		private:
			OperandType::Symbol * target_1_;
			OperandType::Symbol * target_2_;
		public:
			void init(OperandType::Symbol* s1, OperandType::Symbol* s2) override
			{
				target_1_ = s1;
				target_2_ = s2;
			}
			void dump_byte(char buf[]) override
			{

			}
			wstring to_string() override
			{
				return tutils::ascii_string_to_wstring(get_name())
					+ L" " + target_1_->to_string() + L" " + target_2_->to_string();
			}
		};

		class Div : public VirtualDiv
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

		class DivTwo : public VirtualDivTwo
		{
		private:
			OperandType::Symbol * target_1_;
			OperandType::Symbol * target_2_;
		public:
			void init(OperandType::Symbol* s1, OperandType::Symbol* s2) override
			{
				target_1_ = s1;
				target_2_ = s2;
			}
			void dump_byte(char buf[]) override
			{

			}
			wstring to_string() override
			{
				return tutils::ascii_string_to_wstring(get_name())
					+ L" " + target_1_->to_string() + L" " + target_2_->to_string();
			}
		};

		class Mod : public VirtualMod
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

		class ModTwo : public VirtualModTwo
		{
		private:
			OperandType::Symbol * target_1_;
			OperandType::Symbol * target_2_;
		public:
			void init(OperandType::Symbol* s1, OperandType::Symbol* s2) override
			{
				target_1_ = s1;
				target_2_ = s2;
			}
			void dump_byte(char buf[]) override
			{

			}
			wstring to_string() override
			{
				return tutils::ascii_string_to_wstring(get_name())
					+ L" " + target_1_->to_string() + L" " + target_2_->to_string();
			}
		};

		class Add : public VirtualAdd
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

		class Sub : public VirtualSub
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

		class StoreAcc : public VirtualStoreAcc
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

		class GreaterEqual : public VirtualGreaterEqual
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

		class GreaterThan : public VirtualGreaterThan
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

		class LessThan : public VirtualLessThan
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

		class LessEqual : public VirtualLessEqual
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

		class Equal : public VirtualEqual
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

		class NotEqual : public VirtualNotEqual
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

		class And : public VirtualAnd
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

		class Or : public VirtualOr
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
