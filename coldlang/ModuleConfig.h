#pragma once

class ModuleConfig : Config
{
public:
	ModuleConfig(std::string config_addr);
	~ModuleConfig();

	int max_code_length;
	int reserved_code_space;
};

