#include "stdafx.h"
#include "Config.h"
#include "../include/rapidjson/istreamwrapper.h"
#include "../include/rapidjson/pointer.h"
#include <fstream>


int Config::getIntConfig(string pointer)
{
	Value* value = Pointer(pointer.c_str()).Get(this->config_doc);
	assert(value != NULL);
	return value->GetInt();
}

Config::Config(string config_addr) : config_addr(config_addr)
{
	ifstream ifs(config_addr);
	IStreamWrapper isw(ifs);
	config_doc.ParseStream(isw);
}


Config::~Config()
{
}
