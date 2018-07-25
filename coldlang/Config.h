#pragma once
#include "../include/rapidjson/filereadstream.h"
#include "../include/rapidjson/document.h"

class Config
{
private:
	std::string config_addr;
	std::string config_content;
protected:
	rapidjson::Document config_doc;
	int getIntConfig(std::string pointer);
public:
	Config(std::string config_addr);
	~Config();
};

