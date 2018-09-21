#include "stdafx.h"
#include "CppUnitTest.h"
#include  <memory>

#include "../coldlang/ColdLang.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Cld;
using namespace CldRuntime;

namespace unittest
{
	TEST_CLASS(TestIntepreter)
	{
	private:
		typedef unsigned long long int UInt64;
	public:
		TEST_METHOD(Add1) 
		{
			auto val = Interpreter::RunCode<IntegerValue>(LR"X(
				a = 1
				b = 1
				c = a + b
				retv c
			)X");
			Assert::AreEqual((UInt64)val->value, (UInt64)2);
		}
		TEST_METHOD(Add2)
		{
			auto val = Interpreter::RunCode<IntegerValue>(LR"X(
				r1 = 1
				r2 = 1
				r3 = 1
				r4 = 1
				r5 = 1
				r6 = 1
				r7 = 1
				r8 = 1
				r9 = 1
				r10 = 1
				r11 = 1
				r12 = 1
				r13 = 1
				r14 = 1
				r15 = 1
				c = r1 + r2 + r3 + r4 + r5 + r6 + r7 +
					r8 + r9 + r10 + r11 + r12 + r13 + r14 +
					r15
				retv c
			)X");
			Assert::AreEqual((UInt64)val->value, (UInt64)15);
		}
		TEST_METHOD(FuncCall1)
		{
			auto val = Interpreter::RunCode<IntegerValue>(LR"X(
				c = 1
				d = 2
				e = c + d
				native_puts('Fuck you leather man!\n')
				native_puts('Son of a bitch!\n')
				retv e
				
			)X");
			Assert::AreEqual((UInt64)val->value, (UInt64)3);
		}

		TEST_METHOD(FuncCall2)
		{
			auto val = Interpreter::RunCode<IntegerValue>(LR"X(
				c = 1
				d = 2
				native_puts('eat my ass')
				e = c + d
				native_puts('Fuck you leather man!\n')
				native_puts('Son of a bitch!\n')
				e = e + 1
				retv e
				
			)X");
			Assert::AreEqual((UInt64)val->value, (UInt64)4);
		}

		TEST_METHOD(FuncCall3)
		{
			auto val = Interpreter::RunCode<IntegerValue>(LR"X(
				retv native_test_3_args(
					1, 1, 1
				)
				
			)X");
			Assert::AreEqual((UInt64)val->value, (UInt64)3);
		}

		TEST_METHOD(FuncCall4)
		{
			auto val = Interpreter::RunCode<IntegerValue>(LR"X(
				retv native_test_7_args(
					1, 1, 1, 1, 1, 1, 1
				)
				
			)X");
			Assert::AreEqual((UInt64)val->value, (UInt64)7);
		}
	};
}
