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

		Compile::Jit jit(backend->bytecode_reader_);
		jit.Init();
		jit.CompileOne();
		jit.CompileOne();
		Compile::Jit::Entrance entrance = jit.GetEntrance();
		entrance();

		delete tree;
		delete env;
	}
	delete backend;
}

/*
 * from: https://stackoverflow.com/questions/48176431/reading-utf-8-characters-from-console
 */

void init_locale(void)
// Does magic so that wcout can work.
{
#if MS_STDLIB_BUGS
	// Windows needs a little non-standard magic.
	constexpr char cp_utf16le[] = ".1200";
	setlocale(LC_ALL, cp_utf16le);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stdin), _O_WTEXT);
#else
	// The correct locale name may vary by OS, e.g., "en_US.utf8".
	constexpr char locale_name[] = "";
	setlocale(LC_ALL, locale_name);
	std::locale::global(std::locale(locale_name));
	wcout.imbue(std::locale());
	wcin.imbue(std::locale());
#endif
}

int main()
{
	init_locale();
	simple_repl();
	_CrtDumpMemoryLeaks();
    return 0;
}

