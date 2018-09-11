// run.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#ifdef DO_COMPILE

#include "../coldlang/stdafx.h"
#include "../coldlang/ByteCodeClass.h"
#include <ctime>

#include <iostream>
#include <fstream>

#include <fcntl.h>
#include <io.h>
#include <Windows.h>
#include <typeinfo>

#define CRTDBG_MAP_ALLOC  
#include <stdlib.h>
#include <crtdbg.h>
#include "../coldlang/ColdLangBackend.h"
#include <codecvt>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW
#endif
#endif  // _DEBUG

using namespace std;

void test_func_parse1()
{
	wstring code = L"fn class	struct\n"
		L"use if while for retv	task clas f "
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
	for (int i = 0; i < sizeof(ans) / sizeof(ans[0]); i++)
	{
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
	Lexer* lexer;
	wstring code;
	String* str;

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

void test_func_3()
{
	Lexer* lexer;
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

	for (int i = 0; i < sizeof(answers) / sizeof(answers[0]); i++)
	{
		delimiter = (Delimiter *)(lexer->parse_next_token());
		wcout << "i = " << i << "," << answers[i] << "," << delimiter->get_type() << endl;
		delete(delimiter);
	}

	delete(lexer);
}

void test_func_4()
{
	Lexer* lexer;
	wstring code;
	Token* token;

	code = L"num1 / num2 + num3 + \'string\' ";
	lexer = new Lexer(&code);
	//Assert::IsTrue(typeid(lexer->peek_token(0)) == typeid(Word*));
	token = lexer->peek_token(0).get();
	wcout << token->get_raw_string() << endl;
	wcout << typeid(*token).name();
	lexer->next_token();
	//Assert::IsTrue(typeid(lexer->peek_token(1)) == typeid(Word*))
	token = lexer->peek_token(1).get();
	wcout << typeid(*token).name();
	lexer->next_token();
	//Assert::IsTrue(typeid(lexer->peek_token(1)) == typeid(Word*));
	token = lexer->peek_token(1).get();
	wcout << typeid(*token).name();
	//Assert::IsTrue(typeid(lexer->peek_token(3)) == typeid(String*));
	token = lexer->peek_token(4).get();
	wcout << typeid(*token).name();
	wcout << token->get_raw_string();
	lexer->prev_token();
	//Assert::IsTrue(typeid(lexer->peek_token(3)) == typeid(Delimiter*));
	token = lexer->peek_token(3).get();
	wcout << typeid(*token).name();
}

void test_func_5_parse()
{
	wstring code = L"a().b.c().d";
	auto env = new ColdLangFrontEnv(&code);
	wcout << code << endl << endl;

	Tree* tree;
	tree = env->syntax->parse();
	std::wcout << tree->to_xml();
	delete tree;
	delete env;

	code = L"obj1.x_1";
	env = new ColdLangFrontEnv(&code);

	tree = env->syntax->parse();
	std::wcout << tree->to_xml();
	delete tree;
	delete env;

	code = L"a ? b : c ? d : e";
	env = new ColdLangFrontEnv(&code);

	tree = env->syntax->parse();
	std::wcout << tree->to_xml();
	delete tree;
	delete env;

	code = L"a ? b ? c : d : e";
	env = new ColdLangFrontEnv(&code);

	tree = env->syntax->parse();
	std::wcout << tree->to_xml();
	delete tree;
	delete env;

	code = L"a || b && c != 123";
	env = new ColdLangFrontEnv(&code);

	tree = env->syntax->parse();
	std::wcout << tree->to_xml(8);
	delete tree;
	delete env;

	code = L"a == b != \'reset\'";
	env = new ColdLangFrontEnv(&code);

	tree = env->syntax->parse();
	std::wcout << tree->to_xml(8);

	delete tree;
	delete env;

	code = L"a * b-- + +c / d++";
	env = new ColdLangFrontEnv(&code);

	tree = env->syntax->parse();
	std::wcout << tree->to_xml(8);

	delete tree;
	delete env;

	code = L"1.1%2++ - (a % b) - 2";
	env = new ColdLangFrontEnv(&code);

	tree = env->syntax->parse();
	std::wcout << tree->to_xml(8);

	delete tree;
	delete env;
}

void test_func_6_parse_func_def()
{
	wstring code;
	Tree* tree;
	ColdLangFrontEnv* env;

	code = L"fn a { retv a }";
	env = new ColdLangFrontEnv(&code);

	tree = env->syntax->parse();
	std::wcout << tree->to_xml(8);

	delete tree;
	delete env;

	code = L"fn a, b, cde {\n"
		"a = a + 1\n"
		"ret 2 * a }"
		"(\'习近平\', \'习远平\')";
	wcout << code << endl;
	env = new ColdLangFrontEnv(&code);

	tree = env->syntax->parse();
	std::wcout << tree->to_xml(8);

	delete tree;
	delete env;
}

void test_func_6_parse_simple()
{
	wstring code;
	Tree* tree;
	ColdLangFrontEnv* env;

	code = L"a.b";
	env = new ColdLangFrontEnv(&code);

	tree = env->syntax->parse("entity");
	std::wcout << tree->to_xml(100);

	delete tree;
	delete env;
}

void show_name(const char* n)
{
	wcout << n << endl;
}

void test_mem_alloc()
{
	char* big = new char[4 * 1024 * 1024];
	getchar();
	delete[]big;
}

void test_IR_simple()
{
	wstring code;
	Tree* tree;
	ColdLangFrontEnv* env;
	ColdLangBackend* backend;

	code = L"a++";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("factor");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->add(new IR::OperandType::Variable(Word::mock(L"a")));
	//backend->ir_gen->factor_reader(tree->get_root());


	delete tree;
	delete env;
	delete backend;

	code = L"a--";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("factor");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->add(new IR::OperandType::Variable(Word::mock(L"a")));
	//backend->ir_gen->factor_reader(tree->get_root());


	delete tree;
	delete env;
	delete backend;
}

void test_IR_mul()
{
	wstring code;
	Tree* tree;
	ColdLangFrontEnv* env;
	ColdLangBackend* backend;

	code = L"a*b";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("term");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b" });
	backend->ir_gen->term_reader(tree->get_root(), true, "");


	delete tree;
	delete env;
	delete backend;

	code = L"a--*b%c/d++*e";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("term");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c", L"d", L"e" });
	backend->ir_gen->term_reader(tree->get_root(), true, "");

	delete tree;
	delete env;
	delete backend;

	code = L"a";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("term");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a" });
	backend->ir_gen->term_reader(tree->get_root(), true, "");

	delete tree;
	delete env;
	delete backend;
}

void test_IR_add()
{
	wstring code;
	Tree* tree;
	ColdLangFrontEnv* env;
	ColdLangBackend* backend;

	code = L"a+b";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr_5");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b" });
	backend->ir_gen->expr_5_reader(tree->get_root(), true, nullptr, "");

	delete tree;
	delete env;
	delete backend;

	code = L"a*b+c";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr_5");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->expr_5_reader(tree->get_root(), true, nullptr, "");

	delete tree;
	delete env;
	delete backend;

	code = L"a*b+c*d*e+f%g";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr_5");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c", L"d", L"e", L"f", L"g" });
	backend->ir_gen->expr_5_reader(tree->get_root(), true, nullptr, "");

	delete tree;
	delete env;
	delete backend;

	code = L"a-c*d";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr_5");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c", L"d", L"e", L"f", L"g" });
	backend->ir_gen->expr_5_reader(tree->get_root(), true, nullptr, "");

	delete tree;
	delete env;
	delete backend;

	code = L"a*b+c*d*e+f%g-a*b%c";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr_5");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c", L"d", L"e", L"f", L"g" });
	backend->ir_gen->expr_5_reader(tree->get_root(), true, nullptr, "");

	delete tree;
	delete env;
	delete backend;

	code = L"a--+b+c+d+e+f";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr_5");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c", L"d", L"e", L"f", L"g" });
	backend->ir_gen->expr_5_reader(tree->get_root(), true, nullptr, "");

	delete tree;
	delete env;
	delete backend;

	code = L"a*b";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr_5");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c", L"d", L"e", L"f", L"g" });
	backend->ir_gen->expr_5_reader(tree->get_root(), true, nullptr, "");

	delete tree;
	delete env;
	delete backend;
}

void test_IR_compare()
{
	wstring code;
	Tree* tree;
	ColdLangFrontEnv* env;
	ColdLangBackend* backend;

	code = L"a<=b";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr_4");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b" });
	backend->ir_gen->expr_4_reader(tree->get_root(), true, nullptr, "");

	delete tree;
	delete env;
	delete backend;

	code = L"a*b>=c+d";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr_4");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c", L"d" });
	backend->ir_gen->expr_4_reader(tree->get_root(), true, nullptr, "");

	delete tree;
	delete env;
	delete backend;
}

void test_IR_equal()
{
	wstring code;
	Tree* tree;
	ColdLangFrontEnv* env;
	ColdLangBackend* backend;

	code = L"a*b != b-c";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr_3");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->expr_3_reader(tree->get_root(), true, nullptr, "");

	delete tree;
	delete env;
	delete backend;

	code = L"a<=b == b>c";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr_3");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->expr_3_reader(tree->get_root(), true, nullptr, "");

	delete tree;
	delete env;
	delete backend;
}

void test_IR_and_or()
{
	wstring code;
	Tree* tree;
	ColdLangFrontEnv* env;
	ColdLangBackend* backend;

	code = L"a && b || c";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr_1");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->expr_1_reader(tree->get_root(), true, nullptr, "");

	delete tree;
	delete env;
	delete backend;

	code = L"a && b || c <= a";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr_1");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->expr_1_reader(tree->get_root(), true, nullptr, "");

	delete tree;
	delete env;
	delete backend;
}

void test_IR_expr()
{
	wstring code;
	Tree* tree;
	ColdLangFrontEnv* env;
	ColdLangBackend* backend;

	code = L"a*b*c";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->expr_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;

	code = L"a ? b + c : c";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->expr_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;

	code = L"a * b ? b + c : c";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->expr_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;

	code = L"a * b * c ? e ? f : g : h";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c", L"d", L"e", L"f", L"g", L"h" });
	backend->ir_gen->expr_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;
}

void test_IR_statement()
{
	wstring code;
	Tree* tree;
	ColdLangFrontEnv* env;
	ColdLangBackend* backend;

	code = L"a = b + c";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->statement_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;

}

void test_IR_statement_block()
{
	wstring code;
	Tree* tree;
	ColdLangFrontEnv* env;
	ColdLangBackend* backend;

	code = L"a = b + c\n"
			"u = b\n"
			"h = a * u";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement_block");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->statement_block_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;

}

void test_func_def()
{
	wstring code;
	Tree* tree;
	ColdLangFrontEnv* env;
	ColdLangBackend* backend;

	code = L"a, b, c, d, e123";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("comma_identifiers");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->ir_gen->comma_identifiers_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;

	code = L"func = fn a, b, c { a = b + c }";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->ir_gen->statement_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;

	code = L"f = fn param1, param2 { a = a + b   param1 = param2++    } + fn param3 { a = param3 ++ }";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->statement_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;

	code = L"f = fn param1 { func = fn param2 { a = a + b } }";
	wcout << endl << code << endl;
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->statement_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;

	code = L"f = fn param1 { func = fn { } }";
	wcout << endl << code << endl;
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->statement_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;

	code = L"f = fn x { retv fn y { retv x+y } }";
	wcout << endl << code << endl;
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->statement_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;
}

void test_func_call()
{
	wstring code;
	Tree* tree;
	ColdLangFrontEnv* env;
	ColdLangBackend* backend;

	code = L"f = fn x {"
					"retv fn y { retv x+y }"
					" }"
					" a = f(fn {retv a})";
	wcout << endl << code << endl;
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement_block");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->statement_block_reader(tree->get_root());
	backend->function_table->compile_all();

	wcout << "symbol dump: " << endl << "-------------------" << endl;
	wcout << backend->symbol_table->dump_to_string() << "-------------------" << endl;

	delete tree;
	delete env;
	delete backend;


	code = L"f = fn x { ret }"
			"g = fn y { retv y }"
			"h = fn z, w {retv z + w}"
			"a = h(f(a), g(b))"
			"a = a*c*c + b";
	wcout << endl << code << endl;
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement_block");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->statement_block_reader(tree->get_root());
	wcout << "symbol dump: " << endl << "-------------------" << endl;
	wcout << backend->symbol_table->dump_to_string() << "-------------------" << endl;
	backend->function_table->compile_all();

	delete tree;
	delete env;
	delete backend;

	code = L"f = fn x { retv x }"
			"g = fn x { retv x }";
	wcout << endl << code << endl;
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement_block");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->statement_block_reader(tree->get_root());
	backend->function_table->compile_all();

	wcout << "symbol dump: " << endl << "-------------------" << endl;
	wcout << backend->symbol_table->dump_to_string() << "-------------------" << endl;

	delete tree;
	delete env;
	delete backend;
}

void test_simple_run()
{
	wstring code;
	Tree* tree;
	ColdLangFrontEnv* env;
	ColdLangBackend* backend;

	code = L"a = native_puts('boy next door\\n')";
	wcout << endl << code << endl;
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement_block");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->statement_block_reader(tree->get_root());

	Compile::Jit jit(backend->bytecode_reader);
	jit.Init();
	jit.CompileOne();
	jit.CompileOne();
	Compile::Jit::Entrance entrance = jit.GetEntrance();
	entrance();

	delete tree;
	delete env;
	delete backend;
}

void test_simple_call()
{
	wstring code;
	Tree* tree;
	ColdLangFrontEnv* env;
	ColdLangBackend* backend;

	code = 
		L"a = 1"
		"func = fn x { retv x + 1 }"
		"native_puts(func(a))";
	wcout << endl << code << endl;
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement_block");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->statement_block_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;

	code =
		L"a = 1\n"
		"func = fn x { x=x+1 ret }\n"
		"native_puts(func(a))";
	wcout << endl << code << endl;
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement_block");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->statement_block_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;

	code =
		L"a = 1\n"
		"func = fn x {\n x=x+1 \nret\nx=x-1\n }"
		"native_puts(func(a))";
	wcout << endl << code << endl;
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement_block");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->statement_block_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;
}

void test_if_parse()
{
	wstring code;
	Tree* tree;
	ColdLangFrontEnv* env;
	ColdLangBackend* backend;

	code = LR"LINES(
		if a == 1 {
			b = 1
		} else {
			b = 2
		}
	)LINES";

	wcout << endl << code << endl;
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement_block");

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->statement_block_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;

	code = LR"LINES(
		if a == 1 {
			b = 1
		}
	)LINES";

	wcout << endl << code << endl;
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement_block");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->statement_block_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;

	code = LR"LINES(
		if a == 1 {
			b = 1
		} elif c == b {
	        b = 2
	    }
	)LINES";

	wcout << endl << code << endl;
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement_block");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->statement_block_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;

	code = LR"LINES(
		if a == 1 {
			b = 1
		} elif c == b {
	        b = 2
	    } else {
			b = 3
		}
	)LINES";

	wcout << endl << code << endl;
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement_block");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->statement_block_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;

	code = LR"LINES(
		if a == 1 {
			b = 1
		} elif c == b {
	        b = 2
	    } elif true {
			b = 3
		} else {
			b = 4
		}
	)LINES";

	wcout << endl << code << endl;
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement_block");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->statement_block_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;

	code = LR"LINES(
		/*
	     * block comment
		 */
		if true { // line comment
			b = 1
		} elif c == b {
	        b = 2
	    } elif true {
			b = 3
		} else {
			b = 4
		}
	)LINES";

	wcout << endl << code << endl;
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement_block");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->statement_block_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;
}

void test_standard_syntax()
{
	wstring code;
	Tree* tree;
	ColdLangFrontEnv* env;

	auto file_addr = "d:/coldlang/unittest/syntax_standard_test.cld";
	std::ifstream f(file_addr);
	std::wbuffer_convert<codecvt_utf8_utf16<wchar_t>> conv(f.rdbuf());
	std::wistream wf(&conv);

	for (wchar_t c; wf.get(c); ) {
		code.push_back(c);
	}

	wcout << code;

	auto t1 = clock();
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement_block");
	wcout << "parse used: " << clock() - t1 << "ms" << endl;
	//std::wcout << tree->to_xml(100);


	assert(env->lexer->peek_token(1).get() == nullptr);

	delete tree;
	delete env;
}

int main()
{
	_setmode(_fileno(stdout), _O_WTEXT);

	//_CrtSetBreakAlloc(13223);
	test_standard_syntax();
	_CrtDumpMemoryLeaks();
	getchar();

	return 0;
}

#endif