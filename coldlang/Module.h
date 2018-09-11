#pragma once
#include "stdafx.h"
#include "ModuleConfig.h"
#include "CLObject.h"

#include <string>

namespace IR {
	using namespace std;

	class SymbolTable;
	class Function;

	class Module : public IR::CLObject
	{
	private:
		SymbolTable * symbol_table;
		Function * main_function;
		ModuleConfig * config;
		void init();
		wstring get_file_content(string file_addr);
		Function * make_main();
	public:
		Module(const char* file_addr, ModuleConfig * config, 
			SymbolTable * symbol_table);
		string file_addr;
		wstring code;
		virtual wstring to_string() override { return L"module"; };
		Function * get_main() { return main_function; }
		~Module();
	};
}