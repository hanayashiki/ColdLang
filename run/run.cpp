// run.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../coldlang/stdafx.h"

#include <fstream>
#include <iostream>
#include <locale>
#include <codecvt>

#include <fcntl.h>
#include <io.h>
#include <Windows.h>

#define CRTDBG_MAP_ALLOC    
#include <stdlib.h>    
#include <crtdbg.h> 

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW
#endif 
#endif  // _DEBUG

void test_func_parse1()
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
	Lexer* lexer = new Lexer(&code);
	for (int i = 0; i < sizeof(ans) / sizeof(ans[0]); i++) {
		Word* word = (Word*)(lexer->parse_next_token());
		delete(word);
		//Assert::IsTrue(word->get_type() == ans[i]);
	}

	code = L"\'abcdefg\'";
	delete lexer;
	lexer = new Lexer(&code);
	String* str = (String *)(lexer->parse_next_token());
	//Assert::AreEqual(L"abcdefg", str->get_value());
	wcout << str->get_value() << endl;
	wcout << "\\abcd" << endl;
	wcout << L"\'\\\\abcd\'" << endl;
	delete str;
	delete lexer;
}

void test_func_parse2()
{
	Lexer * lexer;
	wstring code;
	String * str;

	code = L"\'\\u0004e60\\u8fd1\\u5e73\'";
	wcout << code << endl;
	lexer = new Lexer(&code);
	str = (String *)(lexer->parse_next_token());
	str->get_value();
	wcout << "unicode is: [" << str->get_value() << "]" << endl;
	delete(lexer);
	delete(str);

	code = L"\'\\u4e60\\u8fd1\\u5e73\'";
	wcout << code << endl;
	lexer = new Lexer(&code);
	str = (String *)(lexer->parse_next_token());
	wcout << str->get_value() << endl;
	delete(lexer);
	delete(str);

	code = L"\'\\u10900\'";
	wcout << code << endl;
	lexer = new Lexer(&code);
	str = (String *)(lexer->parse_next_token());
	wcout << str->get_value() << endl;
	delete(lexer);
	delete(str);
}

void test_func_3() {
	Lexer * lexer;
	wstring code;
	Delimiter* delimiter;

	code = L"+";
	lexer = new Lexer(&code);
	delimiter = (Delimiter *)(lexer->parse_next_token());

	wcout << delimiter->get_type();

	delete(lexer);
	delete(delimiter);
}

int main()
{
	_setmode(_fileno(stdout), _O_WTEXT);

	//_CrtSetBreakAlloc(204);
	test_func_3();
	_CrtDumpMemoryLeaks();
	getchar();
	
	return 0;

}

