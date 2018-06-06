#pragma once
#include "CLObject.h"
#include "Config.h"
#include "ModuleConfig.h"

using namespace std;

class Module :
	public CLObject
{
private:
	ModuleConfig * config;
	void init();
	string get_file_content(string file_addr);
public:
	Module(const char* file_addr, ModuleConfig* config);
	Module(string file_addr, ModuleConfig* config);
	string file_addr;
	string code;
	~Module();
};

