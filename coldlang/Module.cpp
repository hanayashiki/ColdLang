#include "stdafx.h"
#include "Module.h"
#include "Config.h"
#include "ModuleConfig.h"
#include <fstream>


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

string Module::get_file_content(string file_addr) 
{
	ifstream in(file_addr);
	in.seekg(0, ios_base::end);
	std::streamoff file_size = in.tellg();
	if (file_size > config->max_code_length) {
		// TODO
	}
	char *buf = new char[(int)file_size + 1];
	cout << "code len: " << in.tellg() << endl;
	in.seekg(ios_base::beg);
	in.read(buf, file_size);
	in.close();

	buf[file_size] = '\0';
	string code_content(buf);
	// delete buf;
	return code_content;
}

Module::~Module()
{
}
