// run.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../coldlang/stdafx.h"

#include <iostream>


#include <fcntl.h>
#include <io.h>
#include <Windows.h>
#include <typeinfo>

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

	code = L"++	+=+---=	-	*=	*/=/%=	<	<=\n>	>=	==	=	!=&&||!,:+?";
	lexer = new Lexer(&code);
	Delimiter::DelimiterType answers[] = {
		Delimiter::increment,
		Delimiter::add_assign,
		Delimiter::add,
		Delimiter::decrement,
		Delimiter::sub_assign,
		Delimiter::minus,
		Delimiter::mult_assign,
		Delimiter::star,
		Delimiter::div_assign,
		Delimiter::divide,
		Delimiter::mod_assign,
		Delimiter::less,
		Delimiter::less_equal,
		Delimiter::greater,
		Delimiter::greater_equal,
		Delimiter::equal,
		Delimiter::assign,
		Delimiter::not_equal,
		Delimiter::_and,
		Delimiter::_or,
		Delimiter::_not,
		Delimiter::comma,
		Delimiter::colon,
		Delimiter::add,
		Delimiter::question
	};

	for (int i = 0; i < sizeof(answers) / sizeof(answers[0]); i++) {
		delimiter = (Delimiter *)(lexer->parse_next_token());
		wcout << "i = " << i << "," << answers[i] << "," << delimiter->get_type() << endl;
		delete(delimiter);
	}

	delete(lexer);
}

void test_func_4() {
	Lexer * lexer;
	wstring code;
	Token * token;

	code = L"num1 / num2 + num3 + \'string\' ";
	lexer = new Lexer(&code);
	//Assert::IsTrue(typeid(lexer->peek_token(0)) == typeid(Word*));
	token = lexer->peek_token(0);
	wcout << token->get_raw_string() << endl;
	wcout << typeid(*token).name();
	lexer->next_token();
	//Assert::IsTrue(typeid(lexer->peek_token(1)) == typeid(Word*))
	token = lexer->peek_token(1);
	wcout << typeid(*token).name();
	lexer->next_token();
	//Assert::IsTrue(typeid(lexer->peek_token(1)) == typeid(Word*));
	token = lexer->peek_token(1);
	wcout << typeid(*token).name();
	//Assert::IsTrue(typeid(lexer->peek_token(3)) == typeid(String*));
	token = lexer->peek_token(4);
	wcout << typeid(*token).name();
	wcout << token->get_raw_string();
	lexer->prev_token();
	//Assert::IsTrue(typeid(lexer->peek_token(3)) == typeid(Delimiter*));
	token = lexer->peek_token(3);
	wcout << typeid(*token).name();
}


int main()
{
	_setmode(_fileno(stdout), _O_WTEXT);

	//_CrtSetBreakAlloc(204);
	test_func_4();
	_CrtDumpMemoryLeaks();
	getchar();
	
	return 0;

}

