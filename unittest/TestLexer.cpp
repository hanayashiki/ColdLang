#include "stdafx.h"
#include "CppUnitTest.h"
#include "../coldlang/stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unittest
{
	TEST_CLASS(TestLexer)
	{
	public:

		TEST_METHOD(ParseEscapedString)
		{
			Lexer * lexer;
			wstring code;

			code = L"\'abcdefg\'";
			lexer = new Lexer(&code);
			String* str = (String *)(lexer->parse_next_token());
			Assert::AreEqual(L"abcdefg", str->get_value());
			delete(lexer);
			delete(str);

			code = L"\'\'";
			lexer = new Lexer(&code);
			str = (String *)(lexer->parse_next_token());
			Assert::AreEqual(L"", str->get_value());
			delete(lexer);
			delete(str);

			code = L"\'\\\\abcd\'";
			lexer = new Lexer(&code);
			str = (String *)(lexer->parse_next_token());
			Assert::AreEqual(L"\\abcd", str->get_value());
			delete(lexer);
			delete(str);

			code = L"\'西站南广场abcd\\\'abcd\'";
			lexer = new Lexer(&code);
			str = (String *)(lexer->parse_next_token());
			Assert::AreEqual(L"西站南广场abcd\'abcd", str->get_value());
			delete(lexer);
			delete(str);

			code = L"\'西站南广场abcd\\\"abcd\'";
			lexer = new Lexer(&code);
			str = (String *)(lexer->parse_next_token());
			Assert::AreEqual(L"西站南广场abcd\"abcd", str->get_value());
			delete(lexer);
			delete(str);

			code = L"\'西\\a站南广场abcd\\\"abcd\'";
			lexer = new Lexer(&code);
			str = (String *)(lexer->parse_next_token());
			Assert::AreEqual(L"西\a站南广场abcd\"abcd", str->get_value());
			delete(lexer);
			delete(str);

			code = L"\'西\\b站南广场abcd\\\"abcd\\b\'";
			lexer = new Lexer(&code);
			str = (String *)(lexer->parse_next_token());
			Assert::AreEqual(L"西\b站南广场abcd\"abcd\b", str->get_value());
			delete(lexer);
			delete(str);

			code = L"\'西\\b站南广场abcd\\\"abcd\\n\'";
			lexer = new Lexer(&code);
			str = (String *)(lexer->parse_next_token());
			Assert::AreEqual(L"西\b站南广场abcd\"abcd\n", str->get_value());
			delete(lexer);
			delete(str);

			code = L"\'西\\v站南广场abcd\\\"abcd\\v\'";
			lexer = new Lexer(&code);
			str = (String *)(lexer->parse_next_token());
			Assert::AreEqual(L"西\v站南广场abcd\"abcd\v", str->get_value());
			delete(lexer);
			delete(str);

			code = L"\'西\\f站南广场abcd\\\"abcd\\t\'";
			lexer = new Lexer(&code);
			str = (String *)(lexer->parse_next_token());
			Assert::AreEqual(L"西\f站南广场abcd\"abcd\t", str->get_value());
			delete(lexer);
			delete(str);

		}

		TEST_METHOD(ParseWord)
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
			Lexer *lexer = new Lexer(&code);
			for (int i = 0; i < sizeof(ans) / sizeof(ans[0]); i++) {
				Word* word = (Word*)(lexer->parse_next_token());
				Assert::IsTrue(word->get_type() == ans[i]);
			}
			delete(lexer);
		}

		TEST_METHOD(ParseUnicode)
		{
			Lexer * lexer;
			wstring code;
			String* str;

			code = L"\'\\u4e60\\u8fd1\\u5e73\'";
			lexer = new Lexer(&code);
			str = (String *)(lexer->parse_next_token());
			Assert::AreEqual(L"习近平", str->get_value());
			delete(lexer);
			delete(str);

			code = L"\'\\U0000___4e_60 \\u8f_d1\\u5e_73\'";
			lexer = new Lexer(&code);
			str = (String *)(lexer->parse_next_token());
			Assert::AreEqual(L"习 近平", str->get_value());
			delete(lexer);
			delete(str);

			code = L"\'\\t\\U0000___4e_60 \\u8f_d1\\u5e_73\'";
			lexer = new Lexer(&code);
			str = (String *)(lexer->parse_next_token());
			Assert::AreEqual(L"\t习 近平", str->get_value());
			delete(lexer);
			delete(str);

			code = L"\'\\U00010900a\'";
			lexer = new Lexer(&code);
			str = (String *)(lexer->parse_next_token());
			Assert::AreEqual(L"𐤀a", str->get_value());
			delete(lexer);
			delete(str);

			code = L"\'\\x24a\'";
			lexer = new Lexer(&code);
			str = (String *)(lexer->parse_next_token());
			Assert::AreEqual(L"$a", str->get_value());
			delete(lexer);
			delete(str);
		}

	};
}