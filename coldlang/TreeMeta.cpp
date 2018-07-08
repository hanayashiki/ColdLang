#include "stdafx.h"
#include "TreeMeta.h"

TreeMeta::TreeMeta()
{
	// entity -> identifier
	add_builder(new TreeBuilder(
		"entity",
		{ Word::identifier, tutils::negative(Delimiter::left_paren) },
		{ Word::identifier }
	));
	// entity -> function_call
	add_builder(new TreeBuilder(
		"entity",
		{ Word::identifier, Delimiter::left_paren, Delimiter::right_paren },
		{ Word::identifier, Delimiter::left_paren, Delimiter::right_paren }
	));
	add_builder(new TreeBuilder(
		"entity",
		{ Word::identifier, Delimiter::left_paren, Word::identifier },
		{ Word::identifier, Delimiter::left_paren, "entity", Delimiter::right_paren }
	));


}

void TreeMeta::add_builder(TreeBuilder * tree_builder)
{
	if (name_to_builder_.find(tree_builder->get_name()) == name_to_builder_.end())
	{
		name_to_builder_.insert(std::make_pair(tree_builder->get_name(),
			vector<TreeBuilder*>({ tree_builder })
			));
	} else
	{
		name_to_builder_.find(tree_builder->get_name())->second.push_back(tree_builder);
	}
}

vector<TreeBuilder*> & TreeMeta::get_tree_builders(char* name)
{
	// TODO: this is temporary
	return name_to_builder_.find(name)->second;
}

vector<TreeBuilder*> & TreeMeta::get_tree_builders(string name)
{
	// TODO: this is temporary
	return name_to_builder_.find(name)->second;
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
