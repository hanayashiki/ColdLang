#include "stdafx.h"
#include "CppUnitTest.h"

#define private public
#include "../coldlang/stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unittest
{
	TEST_CLASS(TestLexer)
	{
	public:

		TEST_METHOD(ParseNextToken)
		{
			wstring code = L"fn class	struct\n"
				L"use if while for return	task clas f "
				"c88	ccccccccccccccccccasdwasdwdsaggadgegwqfsdvfzb__99852__   _qqq\n";
			Word::WordType ans[] = {
				Word::keyword_fn,
				Word::keyword_class,
				Word::keyword_struct,
				Word::keyword_use,
				Word::keyword_if,
				Word::keyword_while,
				Word::keyword_for,
				Word::keyword_return,
				Word::keyword_task,
				Word::identifier,
				Word::identifier,
				Word::identifier,
				Word::identifier
			};
			Lexer lexer(&code);
			for (int i = 0; i < sizeof(ans) / sizeof(ans[0]); i++) {
				Word* word = (Word*)(lexer.parse_next_token());
				Assert::IsTrue(word->get_type() == ans[i]);
			}
		}



	};
}