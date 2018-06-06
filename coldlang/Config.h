#pragma once
#include "../include/rapidjson/filereadstream.h"
#include "../include/rapidjson/document.h"

using namespace rapidjson;
using namespace std;

class Config
{
private:
	string config_addr;
	string config_content;
protected:
	Document config_doc;
	int getIntConfig(string pointer);
public:
	Config(string config_addr);
	~Config();
};

