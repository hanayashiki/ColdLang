// run.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../coldlang/stdafx.h"

int main()
{
	const char* file_name = "c:/Users/Kiritsugu Emiya/source/repos/coldlang/run/test1.cld";
	Module module(file_name, new ModuleConfig("c:/Users/Kiritsugu Emiya/source/repos/coldlang/coldlang/module.json"));
	cout << module.code.length() << endl;
	cout << module.code << endl;
	getchar();
    return 0;
}

