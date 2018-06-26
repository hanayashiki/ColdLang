﻿// run.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../coldlang/stdafx.h"

#include <fstream>
#include <iostream>
#include <locale>
#include <codecvt>

int main()
{

	const char* file_name = "c:/Users/Kiritsugu Emiya/source/repos/coldlang/run/test1.cld";
	Module module(file_name, new ModuleConfig("c:/Users/Kiritsugu Emiya/source/repos/coldlang/coldlang/module.json"));
	
	locale::global(locale(""));
	wcout.imbue(locale(""));

	wcout << module.code.length() << endl;
	wcout << module.code << endl;

	getchar();
	return 0;
}

