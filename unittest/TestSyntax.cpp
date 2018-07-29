#include "stdafx.h"
#include "CppUnitTest.h"
#include "../coldlang/stdafx.h"
#include "../coldlang/ColdLangBackend.h"
#include <codecvt>
#include <fstream>
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unittest
{
	TEST_CLASS(TestSyntax)
	{
	public:

		TEST_METHOD(StandardSyntax)
		{
			wstring code;
			Tree* tree;
			ColdLangFrontEnv* env;

			auto file_addr = R"(d:\\coldlang\\unittest\\syntax_standard_test.cld)";
			std::ifstream f(file_addr);
			std::wbuffer_convert<codecvt_utf8_utf16<wchar_t>> conv(f.rdbuf());
			std::wistream wf(&conv);

			for (wchar_t c; wf.get(c); ) {
				code.push_back(c);
			}

			Logger::WriteMessage("code: ");
			Logger::WriteMessage(to_string(code.size()).c_str());

			env = new ColdLangFrontEnv(&code);
			tree = env->syntax->parse("statement_block");

			Assert::IsNull(env->lexer->peek_token(0).get());

			delete tree;
			delete env;
		}
	};
}