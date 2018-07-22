// run.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../coldlang/stdafx.h"
#include "../coldlang/ByteCodeClass.h"

#include <iostream>


#include <fcntl.h>
#include <io.h>
#include <Windows.h>
#include <typeinfo>

#define CRTDBG_MAP_ALLOC  
#include <stdlib.h>
#include <crtdbg.h>
#include "../coldlang/ColdLangBackend.h"

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW
#endif
#endif  // _DEBUG

void test_func_parse1()
{
	wstring code = L"fn class	struct\n"
		L"use if while for ret	task clas f "
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

	code = L"fn a { ret a }";
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

void test_ir_marcos()
{
//#define SHOW_NAME(name, ...) \
//	show_name(#name);
//
//	BYTECODE_LIST(SHOW_NAME)
//
//#undef SHOW_NAME

	auto cadd = IR::BytecodeClass::LoadAttributeToAcc();
	wcout << cadd.get_name() << endl;
	wcout << cadd.get_id() << endl;

	auto ccall = IR::BytecodeClass::CallFunc();
	wcout << ccall.get_name() << endl;
	wcout << ccall.get_id() << endl;

	wcout << sizeof(int*) << endl;
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
	backend->symbol_table_->add(new IR::OperandType::Variable(Word::mock(L"a")));
	//backend->ir_gen_->factor_reader(tree->get_root());


	delete tree;
	delete env;
	delete backend;

	code = L"a--";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("factor");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table_->add(new IR::OperandType::Variable(Word::mock(L"a")));
	//backend->ir_gen_->factor_reader(tree->get_root());


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
	backend->symbol_table_->mock({ L"a", L"b" });
	backend->ir_gen_->term_reader(tree->get_root(), true, "");


	delete tree;
	delete env;
	delete backend;

	code = L"a--*b%c/d++*e";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("term");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table_->mock({ L"a", L"b", L"c", L"d", L"e" });
	backend->ir_gen_->term_reader(tree->get_root(), true, "");

	delete tree;
	delete env;
	delete backend;

	code = L"a";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("term");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table_->mock({ L"a" });
	backend->ir_gen_->term_reader(tree->get_root(), true, "");

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
	backend->symbol_table_->mock({ L"a", L"b" });
	backend->ir_gen_->expr_5_reader(tree->get_root(), true, nullptr, "");

	delete tree;
	delete env;
	delete backend;

	code = L"a*b+c";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr_5");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table_->mock({ L"a", L"b", L"c" });
	backend->ir_gen_->expr_5_reader(tree->get_root(), true, nullptr, "");

	delete tree;
	delete env;
	delete backend;

	code = L"a*b+c*d*e+f%g";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr_5");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table_->mock({ L"a", L"b", L"c", L"d", L"e", L"f", L"g" });
	backend->ir_gen_->expr_5_reader(tree->get_root(), true, nullptr, "");

	delete tree;
	delete env;
	delete backend;

	code = L"a-c*d";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr_5");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table_->mock({ L"a", L"b", L"c", L"d", L"e", L"f", L"g" });
	backend->ir_gen_->expr_5_reader(tree->get_root(), true, nullptr, "");

	delete tree;
	delete env;
	delete backend;

	code = L"a*b+c*d*e+f%g-a*b%c";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr_5");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table_->mock({ L"a", L"b", L"c", L"d", L"e", L"f", L"g" });
	backend->ir_gen_->expr_5_reader(tree->get_root(), true, nullptr, "");

	delete tree;
	delete env;
	delete backend;

	code = L"a--+b+c+d+e+f";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr_5");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table_->mock({ L"a", L"b", L"c", L"d", L"e", L"f", L"g" });
	backend->ir_gen_->expr_5_reader(tree->get_root(), true, nullptr, "");

	delete tree;
	delete env;
	delete backend;

	code = L"a*b";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr_5");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table_->mock({ L"a", L"b", L"c", L"d", L"e", L"f", L"g" });
	backend->ir_gen_->expr_5_reader(tree->get_root(), true, nullptr, "");

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
	backend->symbol_table_->mock({ L"a", L"b" });
	backend->ir_gen_->expr_4_reader(tree->get_root(), true, nullptr, "");

	delete tree;
	delete env;
	delete backend;

	code = L"a*b>=c+d";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr_4");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table_->mock({ L"a", L"b", L"c", L"d" });
	backend->ir_gen_->expr_4_reader(tree->get_root(), true, nullptr, "");

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
	backend->symbol_table_->mock({ L"a", L"b", L"c" });
	backend->ir_gen_->expr_3_reader(tree->get_root(), true, nullptr, "");

	delete tree;
	delete env;
	delete backend;

	code = L"a<=b == b>c";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr_3");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table_->mock({ L"a", L"b", L"c" });
	backend->ir_gen_->expr_3_reader(tree->get_root(), true, nullptr, "");

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
	backend->symbol_table_->mock({ L"a", L"b", L"c" });
	backend->ir_gen_->expr_1_reader(tree->get_root(), true, nullptr, "");

	delete tree;
	delete env;
	delete backend;

	code = L"a && b || c <= a";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr_1");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table_->mock({ L"a", L"b", L"c" });
	backend->ir_gen_->expr_1_reader(tree->get_root(), true, nullptr, "");

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
	backend->symbol_table_->mock({ L"a", L"b", L"c" });
	backend->ir_gen_->expr_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;

	code = L"a ? b + c : c";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table_->mock({ L"a", L"b", L"c" });
	backend->ir_gen_->expr_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;

	code = L"a * b ? b + c : c";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr");
	std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table_->mock({ L"a", L"b", L"c" });
	backend->ir_gen_->expr_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;

	code = L"a * b * c ? e ? f : g : h";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("expr");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table_->mock({ L"a", L"b", L"c", L"d", L"e", L"f", L"g", L"h" });
	backend->ir_gen_->expr_reader(tree->get_root());

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
	backend->symbol_table_->mock({ L"a", L"b", L"c" });
	backend->ir_gen_->statement_reader(tree->get_root());

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
	backend->symbol_table_->mock({ L"a", L"b", L"c" });
	backend->ir_gen_->statement_block_reader(tree->get_root());

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
	backend->ir_gen_->comma_identifiers_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;

	code = L"func = fn a, b, c { a = b + c }";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->ir_gen_->statement_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;

	code = L"f = fn param1, param2 { a = a + b   param1 = param2++    } + fn param3 { a = param3 ++ }";
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table_->mock({ L"a", L"b", L"c" });
	backend->ir_gen_->statement_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;

	code = L"f = fn param1 { func = fn param2 { a = a + b } }";
	wcout << endl << code << endl;
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table_->mock({ L"a", L"b", L"c" });
	backend->ir_gen_->statement_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;

	code = L"f = fn param1 { func = fn { } }";
	wcout << endl << code << endl;
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table_->mock({ L"a", L"b", L"c" });
	backend->ir_gen_->statement_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;

	code = L"f = fn x { ret fn y { ret x+y } }";
	wcout << endl << code << endl;
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table_->mock({ L"a", L"b", L"c" });
	backend->ir_gen_->statement_reader(tree->get_root());

	delete tree;
	delete env;
	delete backend;
}

int main()
{
	_setmode(_fileno(stdout), _O_WTEXT);

	//_CrtSetBreakAlloc(2093);
	test_func_def();
	_CrtDumpMemoryLeaks();
	getchar();

	return 0;
}
