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
	using namespace IR::BytecodeClass;

	TEST_CLASS(TestTypeCalc)
	{
	public:		
		TEST_METHOD(type_calc_add)
		{
			Assert::IsTrue(TypeCalc::type_calc_add(IntegerVal, IntegerVal) == IntegerVal);
			Assert::IsTrue(TypeCalc::type_calc_add(FloatVal, IntegerVal) == FloatVal);
			Assert::IsTrue(TypeCalc::type_calc_add(StringPointerVal, IntegerVal) == StringPointerVal);
			Assert::IsTrue(TypeCalc::type_calc_add(ObjectPointerVal, IntegerVal) == AnyVal);
			Assert::IsTrue(TypeCalc::type_calc_add(IntegerVal, ObjectPointerVal) == AnyVal);
		}
		TEST_METHOD(type_calc_mult_div)
		{
			Assert::IsTrue(TypeCalc::type_calc_mult_div(IntegerVal, ObjectPointerVal) == AnyVal);
			Assert::IsTrue(TypeCalc::type_calc_mult_div(ObjectPointerVal, IntegerVal) == AnyVal);
			Assert::IsTrue(TypeCalc::type_calc_mult_div(IntegerVal, IntegerVal) == IntegerVal);
			Assert::IsTrue(TypeCalc::type_calc_mult_div(FloatVal, FloatVal) == FloatVal);
			Assert::IsTrue(TypeCalc::type_calc_mult_div(IntegerVal, FloatVal) == FloatVal);
			Assert::IsTrue(TypeCalc::type_calc_mult_div(StringPointerVal, FloatVal) == ErrorVal);
		}
	};
}