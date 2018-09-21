// run.cpp: 定义控制台应用程序的入口点。
//
#include "stdafx.h"

//#define DO_COMPILE

#ifdef DO_COMPILE

#include "../coldlang/stdafx.h"
#include "../coldlang/ByteCodeClass.h"
#include "../coldlang/Module.h"
#include "../coldlang/log.h"
#include <ctime>

#include <iostream>
#include <fstream>

#include <fcntl.h>
#include <io.h>
#include <Windows.h>
#include <typeinfo>

#define CRTDBG_MAP_ALLOC  
#include <cstdlib>
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

	CLD_DEBUG << "rf.run() started" << std::endl;
	rf.Run({});

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
	IR::Module* module;

	code = LR"LINES(
		native_puts('Fuck you leather man!\n')
		native_puts('Son of a bitch!\n')
		retv 1
	)LINES";
	env = new ColdLangFrontEnv(&code);
	CLD_DEBUG << "parse started" << std::endl;
	tree = env->syntax->parse("statement_block");
	//std::wcout << tree->to_xml(100);

	backend = new ColdLangBackend();
	module = new IR::Module("test", nullptr, backend->symbol_table);
	backend->ir_gen->statement_block_reader(tree->get_root());

	blocks = new IR::BasicBlockHolder(backend->bytecode_reader);
	CLD_DEBUG << "make_blocks" << std::endl;
	blocks->make_blocks();
	// wcout << blocks->to_string();

	CldRuntime::RuntimeFunction rf(&blocks->get_blocks(), module->get_main());

	CLD_DEBUG << LOG_EXPR(backend->symbol_table->dump_to_string()) << std::endl;
	CLD_DEBUG << "rf run" << std::endl;
	rf.Run({});

	CLD_DEBUG << backend->symbol_table->dump_to_string() << std::endl;

	delete tree;
	delete env;
	delete backend;
	delete blocks;
	delete module;

	CLD_DEBUG << "rf run complete" << std::endl;
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// _CrtSetBreakAlloc(2295);
	test_simple_eval_calc_run();
	getchar();
	return 0;
}

#endif