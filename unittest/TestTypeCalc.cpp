#include "stdafx.h"
#include "CppUnitTest.h"
#include "../coldlang/stdafx.h"
#include "../coldlang/BytecodeVectorReader.h"
#include "../coldlang/TypeCalc.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unittest
{
	using namespace IR;
	using namespace Runtime;
	using namespace BytecodeClass;

	TEST_CLASS(TestTypeCalc)
	{
	public:		
		TEST_METHOD(TestGetResultType)
		{
			Assert::IsTrue(TypeCalc::get_result_type(OpAdd, IntegerVal, IntegerVal) == IntegerVal);
			Assert::IsTrue(TypeCalc::get_result_type(OpAdd, IntegerVal, FloatVal) == FloatVal);
			Assert::IsTrue(TypeCalc::get_result_type(OpMul, IntegerVal, StringPointerVal) == StringPointerVal);
			Assert::IsTrue(TypeCalc::get_result_type(OpMul, FloatVal, StringPointerVal) == ErrorVal);
		}
	};
}