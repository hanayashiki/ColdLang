#include "stdafx.h"
#include "Module.h"
#include "Config.h"
#include "ModuleConfig.h"
#include <fstream>
#include <iostream>
#include <codecvt>

Module::Module(const char *file_addr, ModuleConfig * config): config(config)
{
	this->file_addr = file_addr;
	init();
}

Module::Module(string file_addr, ModuleConfig * config): file_addr(file_addr), config(config)
{
	init();
}

void Module::init() 
{
	this->code = get_file_content(this->file_addr);
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

Module::~Module()
{
}
