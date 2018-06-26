#include "stdafx.h"
#include "CppUnitTest.h"

#include "../coldlang/stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unittest
{		
	TEST_CLASS(TestToken)
	{
	public:
		
		TEST_METHOD(TestTokenOnString)
		{
			Token token(L"abc…µ±∆", 1, 2);
			Assert::AreEqual(L"abc…µ±∆", token.get_raw_string());
			Assert::AreNotEqual(L"abc…µ±∆1", token.get_raw_string());
		}

	};
}