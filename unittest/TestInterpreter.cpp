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
	};
}
