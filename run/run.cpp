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

int main()
{

	const char* file_name = "./test1.cld";
	Module module(file_name, new ModuleConfig("../coldlang/module.json"));
	//
	_setmode(_fileno(stdout), _O_WTEXT);

	wstring code = L"fn class	struct\n"
		L"use if while for return	task\n";
	Word::WordType ans[] = {
		Word::keyword_fn,
		Word::keyword_class,
		Word::keyword_struct,
		Word::keyword_use,
		Word::keyword_if,
		Word::keyword_while,
		Word::keyword_for,
		Word::keyword_return,
		Word::keyword_task
	};
	Lexer lexer(&code);
	for (int i = 0; i < sizeof(ans) / sizeof(ans[0]); i++) {
		Word* word = (Word*)(lexer.parse_next_token());

	}

	getchar();
	return 0;
}

