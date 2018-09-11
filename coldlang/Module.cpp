#include "stdafx.h"
#include "Module.h"
#include "Config.h"
#include <fstream>
#include <iostream>
#include <codecvt>

namespace IR 
{
	Module::Module(const char *file_addr, ModuleConfig * config, SymbolTable * symbol_table) 
		: config(config), symbol_table(symbol_table)
	{
		// this->file_addr = file_addr;
		init();
	}

	void Module::init()
	{
		// code = get_file_content(this->file_addr);
		main_function = make_main();
	}

	wstring Module::get_file_content(string file_addr)
	{
		std::ifstream f(file_addr);
		std::wbuffer_convert<std::codecvt_utf8_utf16<wchar_t>> conv(f.rdbuf());
		std::wistream wf(&conv);

		wstring code;
		code.reserve(config->reserved_code_space);

		for (wchar_t c; wf.get(c); ) {
			code.push_back(c);
		}
		return code;
	}

	Function * Module::make_main()
	{
		auto token = Word::mockAsToken(L"%main");
		vector<OperandType::Variable*> parameters;
		return new Function(
			token,
			parameters,
			symbol_table
		);
	}

	Module::~Module()
	{
		delete main_function;
	}
}