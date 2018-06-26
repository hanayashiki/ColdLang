#include "stdafx.h"
#include "ModuleConfig.h"
#include "../include/rapidjson/document.h"
#include "../include/rapidjson/pointer.h"


ModuleConfig::ModuleConfig(string config_addr) : Config(config_addr)
{
	this->max_code_length = this->getIntConfig("/max_code_length");
	this->reserved_code_space = this->getIntConfig("/reserved_code_space");
}


ModuleConfig::~ModuleConfig()
{
}
