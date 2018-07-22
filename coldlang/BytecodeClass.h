#pragma once
#include "stdafx.h"

namespace IR
{
	namespace BytecodeClass
	{
		using namespace OperandType;
		class LoadAttributeToAcc : public VirtualLoadAttributeToAcc
		{
		public:
			void init(Symbol* symbol, Index* index) override
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
			Symbol * target_;
		public:
			void init(Symbol* t) override
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

		class LoadFunctionToAcc : public VirtualLoadFunctionToAcc
		{
		private:
			Function * target_;
		public:
			void init(Function* t) override
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
			void init(Index*) override
			{
				wcout << "fuck you this is not implemented" << endl;
			}
			void dump_byte(char buf[]) override
			{

			}
		};

		class RetAcc : public VirtualRetAcc
		{
		public:
			void init() override
			{
			}
			void dump_byte(char buf[]) override
			{

			}
			wstring to_string() override
			{
				return tutils::ascii_string_to_wstring(get_name());
			}
		};

		class Inc : public VirtualInc
		{
		private:
			Symbol * target_;
		public:
			void init(Symbol* s) override
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
			Symbol * target_;
		public:
			void init(Symbol* s) override
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
			Symbol * target_;
		public:
			void init(Symbol* s) override
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
			Symbol * target_1_;
			Symbol * target_2_;
		public:
			void init(Symbol* s1, Symbol* s2) override
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
			Symbol * target_;
		public:
			void init(Symbol* s) override
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
			Symbol * target_1_;
			Symbol * target_2_;
		public:
			void init(Symbol* s1, Symbol* s2) override
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
			Symbol * target_;
		public:
			void init(Symbol* s) override
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
			Symbol * target_1_;
			Symbol * target_2_;
		public:
			void init(Symbol* s1, Symbol* s2) override
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
			Symbol * target_;
		public:
			void init(Symbol* s) override
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
			Symbol * target_;
		public:
			void init(Symbol* s) override
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
			Symbol * target_;
		public:
			void init(Symbol* s) override
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
			Symbol * target_;
		public:
			void init(Symbol* s) override
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
			Symbol * target_;
		public:
			void init(Symbol* s) override
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
			Symbol * target_;
		public:
			void init(Symbol* s) override
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
			Symbol * target_;
		public:
			void init(Symbol* s) override
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
			Symbol * target_;
		public:
			void init(Symbol* s) override
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
			Symbol * target_;
		public:
			void init(Symbol* s) override
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
			Symbol * target_;
		public:
			void init(Symbol* s) override
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
			Symbol * target_;
		public:
			void init(Symbol* s) override
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

		class JumpOnTrue : public VirtualJumpOnTrue
		{
		private:
			Label * target_;
		public:
			void init(Label* target) override
			{
				target_ = target;
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

		class JumpOnFalse : public VirtualJumpOnFalse
		{
		private:
			Label * target_;
		public:
			void init(Label * target) override
			{
				target_ = target;
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

		class Jump : public VirtualJump
		{
		private:
			Label * target_;
		public:
			void init(OperandType::Label* target) override
			{
				target_ = target;
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
