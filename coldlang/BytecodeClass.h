#pragma once
#include "stdafx.h"
#include "NativeFunction.h"
#include "BytecodeByteDump.h"
#include "Literal.h"

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
			wstring to_string() override
			{
				return tutils::ascii_string_to_wstring(get_name())
					+ L" " + target_->to_string();
			}
		};

		class LoadFunctionToAcc : public VirtualLoadFunctionToAcc
		{
		private:
			Literal * target_;
		public:
			void init(Literal * t) override
			{
				target_ = t;
			}
			wstring to_string() override
			{
				return tutils::ascii_string_to_wstring(get_name())
					+ L" " + target_->to_string();
			}
		};

		class CallFunc : public VirtualCallFunc
		{
		private:
			Symbol * target_;
		public:
			void init(Symbol* target) override
			{
				target_ = target;
			}
			wstring to_string() override
			{
				return tutils::ascii_string_to_wstring(get_name())
					+ L" " + target_->to_string();
			}
		};

		class CallNativeFunc : public VirtualCallNativeFunc
		{
		private:
			NativeFunction * target_;
		public:
			void init(NativeFunction* target) override
			{
				target_ = target;
			}

			void generate_byte(NativeFunction* target) override
			{
				byte_len = ByteDump::dump_byte(byte_buf, EnumCallNativeFunc, target);
			}
			
			wstring to_string() override
			{
				return tutils::ascii_string_to_wstring(get_name())
					+ L" " + target_->to_string();
			}
		};

		class PushParamVariable : public VirtualPushParamVariable
		{
		private:
			Variable * target_;
		public:
			void init(Variable* target) override
			{
				target_ = target;
			}

			void generate_byte(Variable* target) override
			{
				byte_len = ByteDump::dump_byte(byte_buf, EnumPushParamVariable, target);
			}

			wstring to_string() override
			{
				return tutils::ascii_string_to_wstring(get_name())
					+ L" " + target_->to_string();
			}
		};

		class PushParamLiteral : public VirtualPushParamLiteral
		{
		private:
			Literal * target_;
		public:
			void init(Literal* target) override
			{
				target_ = target;
			}

			void generate_byte(Literal* target) override
			{
				byte_len = ByteDump::dump_byte(byte_buf, EnumPushParamLiteral, target);
			}

			wstring to_string() override
			{
				return tutils::ascii_string_to_wstring(get_name())
					+ L" " + target_->to_string();
			}
		};

		class RetAcc : public VirtualRetAcc
		{
		public:
			void init() override
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
			
			wstring to_string() override
			{
				return tutils::ascii_string_to_wstring(get_name())
					+ L" " + target_->to_string();
			}
		};
	}
}
