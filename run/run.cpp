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

	//const char* file_name = "./test1.cld";
	//Module module(file_name, new ModuleConfig("../coldlang/module.json"));
	//
	_setmode(_fileno(stdout), _O_WTEXT);

	/*wcout << module.code.length() << endl;
	wcout << module.code << endl;*/

	Trie<TrieNode<wchar_t, 26>, wchar_t, 10> trie(L'a');
	trie.add_sequence(L"abcd", 4);
	trie.add_sequence(L"bcde", 4);
	trie.add_sequence(L"bgh", 3);

	wcout << trie.get_sequence_tag(L"bcde", 4);

	getchar();
	return 0;
}

