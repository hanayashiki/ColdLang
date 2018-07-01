#pragma once
#include "stdafx.h"
#include <unordered_map>

class TreeMeta
{
private:
	std::unordered_map<string, std::vector<TreeBuilder*>> name_to_builder_;
public:
	void add_builder(TreeBuilder*);
	vector<TreeBuilder*> & get_tree_builders(char* name);
	vector<TreeBuilder*> & get_tree_builders(string name);
	~TreeMeta();
};
