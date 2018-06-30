#include "stdafx.h"
#include "TreeMeta.h"

void TreeMeta::add_builder(TreeBuilder * tree_builder)
{
	if (name_to_builder_.find(tree_builder->get_name()) != name_to_builder_.end())
	{
		name_to_builder_.insert(std::make_pair(tree_builder->get_name(),
			vector<TreeBuilder*>({ tree_builder })
			));
	} else
	{
		name_to_builder_.find(tree_builder->get_name())->second.push_back(tree_builder);
	}
}

TreeMeta::~TreeMeta()
{
	for (auto p : name_to_builder_)
	{
		for (auto tb : p.second) {
			delete tb;
		}
	}
}
