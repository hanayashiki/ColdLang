#pragma once
#include "CLObject.h"
#include "Config.h"
#include "ModuleConfig.h"

namespace IR {
	using namespace std;

	class Module : public IR::CLObject
	{
	private:
		ModuleConfig * config;
		void init();
		wstring get_file_content(string file_addr);
	public:
		Module(const char* file_addr, ModuleConfig* config);
		Module(string file_addr, ModuleConfig* config);
		string file_addr;
		wstring code;
		~Module();
	};
}