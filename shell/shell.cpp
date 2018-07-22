// shell.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../coldlang/stdafx.h"
#include <iostream>

#include <fcntl.h>
#include <io.h>

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

void simple_repl()
{
	ColdLangBackend* backend = new ColdLangBackend();
	backend->symbol_table_->mock({ L"a", L"b", L"c" });

	while (true)
	{
		wstring code;
		getline(wcin, code);

		if (code == L":q")
		{
			break;
		}
		Tree* tree;
		ColdLangFrontEnv* env;

		env = new ColdLangFrontEnv(&code);
		tree = env->syntax->parse("statement");
		// std::wcout << tree->to_xml(100);

		backend->ir_gen_->statement_reader(tree->get_root());

		delete tree;
		delete env;
	}
	delete backend;
}

int main()
{
	_setmode(_fileno(stdout), _O_WTEXT);
	simple_repl();
	_CrtDumpMemoryLeaks();
    return 0;
}

