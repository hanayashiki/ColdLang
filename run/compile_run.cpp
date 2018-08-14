// run.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
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
#include "../coldlang/BasicBlockHolder.h"
#include "../coldlang/RuntimeFunction.h"

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW
#endif
#endif  // _DEBUG

using namespace std;

void test_simple_calculation()
{
	wstring code;
	Tree* tree;
	ColdLangFrontEnv* env;
	ColdLangBackend* backend;

	code =
		L"a = 1 + 1";
	wcout << endl << code << endl;
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement_block");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->statement_block_reader(tree->get_root());
	backend->typed_block_compiler_caller->NextBlock();

	delete tree;
	delete env;
	delete backend;

	code =
		L"a = 1*2 + 2*3";
	wcout << endl << code << endl;
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement_block");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->statement_block_reader(tree->get_root());
	backend->typed_block_compiler_caller->NextBlock();

	delete tree;
	delete env;
	delete backend;
}

void test_blockize()
{
	wstring code;
	Tree* tree;
	ColdLangFrontEnv* env;
	ColdLangBackend* backend;
	IR::BasicBlockHolder* blocks;

	code = LR"LINES(
		a = 1 + 1
		b = a + 1
		c = a + b
	)LINES";
	wcout << endl << code << endl;
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement_block");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->statement_block_reader(tree->get_root());

	blocks = new IR::BasicBlockHolder(backend->bytecode_reader);
	blocks->make_blocks();
	wcout << blocks->to_string();

	delete tree;
	delete env;
	delete backend;
	delete blocks;

	code = LR"LINES(
a = 2
if a > 3 {
	b = 1
} else {
	b = 0
}
	)LINES";
	wcout << endl << code << endl;
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement_block");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->symbol_table->mock({ L"a", L"b", L"c" });
	backend->ir_gen->statement_block_reader(tree->get_root());

	blocks = new IR::BasicBlockHolder(backend->bytecode_reader);
	blocks->make_blocks();
	wcout << blocks->to_string();

	delete tree;
	delete env;
	delete backend;
	delete blocks;
}

void test_simple_calc_run()
{
	wstring code;
	Tree* tree;
	ColdLangFrontEnv* env;
	ColdLangBackend* backend;
	IR::BasicBlockHolder* blocks;

	code = LR"LINES(
		a = 1 + 1
	)LINES";
	wcout << endl << code << endl;
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement_block");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->ir_gen->statement_block_reader(tree->get_root());

	blocks = new IR::BasicBlockHolder(backend->bytecode_reader);
	blocks->make_blocks();
	// wcout << blocks->to_string();

	CldRuntime::RuntimeFunction rf(&blocks->get_blocks(), nullptr);
	rf.run();

	delete tree;
	delete env;
	delete backend;
	delete blocks;

}

void test_simple_eval_calc_run()
{
	wstring code;
	Tree* tree;
	ColdLangFrontEnv* env;
	ColdLangBackend* backend;
	IR::BasicBlockHolder* blocks;

	code = LR"LINES(
		a = 1 + 1
	)LINES";
	wcin >> code; getchar();
	env = new ColdLangFrontEnv(&code);
	tree = env->syntax->parse("statement_block");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	backend->ir_gen->statement_block_reader(tree->get_root());

	blocks = new IR::BasicBlockHolder(backend->bytecode_reader);
	blocks->make_blocks();
	// wcout << blocks->to_string();

	CldRuntime::RuntimeFunction rf(&blocks->get_blocks(), nullptr);
	rf.run();

	delete tree;
	delete env;
	delete backend;
	delete blocks;

}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// _CrtSetBreakAlloc(6841);
	for (int i = 0; i < 1000; i++)
		test_simple_calc_run();
	//getchar();
	return 0;
}