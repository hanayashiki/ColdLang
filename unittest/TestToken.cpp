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
			wchar_t* test_string = new wchar_t[100];
			wcsncpy_s(test_string, 100, L"abc草泥马", 100);

			Token token(nullptr, test_string, 1, 2);
			Assert::AreEqual(L"abc草泥马", token.get_raw_string());
			Assert::AreNotEqual(L"abc草泥马1", token.get_raw_string());
		}

	};
}