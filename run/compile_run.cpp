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
	backend->typed_block_compiler_caller->next_block();

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
	backend->typed_block_compiler_caller->next_block();

	delete tree;
	delete env;
	delete backend;
}

int main()
{
	test_simple_calculation();
	_CrtDumpMemoryLeaks();
	getchar();
	return 0;
}