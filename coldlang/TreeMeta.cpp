#include "stdafx.h"
#include "TreeMeta.h"

TreeMeta::TreeMeta()
{
	add_entity_defs();

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

void TreeMeta::add_expr_defs()
{
// TODO: after peek
}

void TreeMeta::add_entity_defs()
{
	// atom -> identifier
	add_builder(new TreeBuilder(
		"atom",
		{ Word::identifier, tutils::negative(Delimiter::left_paren) },
		{ Word::identifier }
	));

	// atom -> func_call
	add_builder(new TreeBuilder(
		"atom",
		{ Word::identifier, Delimiter::left_paren },
		{ Word::identifier, Delimiter::left_paren, Delimiter::right_paren }
	));

	// entity -> atom -> string
	add_builder(new TreeBuilder(
		"entity",
		{ String::GeneralString },
		{ String::GeneralString }
	));

	// entity -> atom -> integer
	add_builder(new TreeBuilder(
		"entity",
		{ "integer" },
		{ "integer" }
	));

	// entity -> atom -> float
	add_builder(new TreeBuilder(
		"entity",
		{ "float" },
		{ "float" }
	));

	// entity -> atom sub_entity
	add_builder(new TreeBuilder(
		"entity",
		{ Word::identifier },
		{ "atom", "sub_entity" }
	));

	// sub_entity -> `.` entity
	add_builder(new TreeBuilder(
		"sub_entity",
		{ Delimiter::period },
		{ Delimiter::period, "entity" }
	));

	// sub_entity -> None
	add_builder(new TreeBuilder(
		"sub_entity",
		{ tutils::negative(Delimiter::period) },
		{}
	));
}