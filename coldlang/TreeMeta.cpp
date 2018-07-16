#include "stdafx.h"
#include "TreeMeta.h"

TreeMeta::TreeMeta()
{
	add_entity_defs();
	add_expr_defs();
	add_func_defs();
	add_statement_defs();
}

void TreeMeta::add_builder(TreeBuilder * tree_builder)
{
	if (name_to_builder_.find(tree_builder->get_name()) == name_to_builder_.end())
	{
		name_to_builder_.insert(std::make_pair(tree_builder->get_name(),
			vector<TreeBuilder*>({ tree_builder })
		));
	}
	else
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
	// expr -> expr_1 `?` expr `:` expr
	add_builder(new TreeBuilder(
		"expr",
		{ expr_peek_ },
		{ "expr_1", "expr_tail" }
	));
	add_builder(new TreeBuilder(
		"expr_tail",
		{ Delimiter::question },
		{ Delimiter::question, "expr", Delimiter::colon, "expr" }
	));
	add_builder(new TreeBuilder(
		"expr_tail",
		{ },
		{ }
	));

	// expr_1 -> expr_2 { `||` expr_2 }

	add_builder(new TreeBuilder(
		"expr_1",
		{ expr_peek_ },
		{ "expr_2", "expr_1_tail" }
	));

	add_builder(new TreeBuilder(
		"expr_1_tail",
		{ Delimiter::_or },
		{ Delimiter::_or, "expr_1" }
	));

	add_builder(new TreeBuilder(
		"expr_1_tail",
		{ },
		{ }
	));

	// expr_2 -> expr_3 { `&&` expr_3 }

	add_builder(new TreeBuilder(
		"expr_2",
		{ expr_peek_ },
		{ "expr_3", "expr_2_tail" }
	));

	add_builder(new TreeBuilder(
		"expr_2_tail",
		{ Delimiter::_and },
		{ Delimiter::_and, "expr_2" }
	));

	add_builder(new TreeBuilder(
		"expr_2_tail",
		{},
		{}
	));

	// expr_3 -> expr_4 { (`==`|`!=`) expr_4 }

	add_builder(new TreeBuilder(
		"expr_3",
		{ expr_peek_ },
		{ "expr_4", "expr_3_tail" }
	));

	add_builder(new TreeBuilder(
		"expr_3_tail",
		{ Delimiter::not_equal },
		{ Delimiter::not_equal, "expr_3" }
	));

	add_builder(new TreeBuilder(
		"expr_3_tail",
		{ Delimiter::equal },
		{ Delimiter::equal, "expr_3" }
	));

	add_builder(new TreeBuilder(
		"expr_3_tail",
		{},
		{}
	));

	// expr_4 -> expr_5 { (`>=`|`>`|`<`|`<=`) expr_5 }

	add_builder(new TreeBuilder(
		"expr_4",
		{ expr_peek_ },
		{ "expr_5", "expr_4_tail" }
	));

	add_builder(new TreeBuilder(
		"expr_4_tail",
		{ Delimiter::greater_equal },
		{ Delimiter::greater_equal, "expr_4" }
	));

	add_builder(new TreeBuilder(
		"expr_4_tail",
		{ Delimiter::greater },
		{ Delimiter::greater, "expr_4" }
	));

	add_builder(new TreeBuilder(
		"expr_4_tail",
		{ Delimiter::less_equal },
		{ Delimiter::less_equal, "expr_4" }
	));

	add_builder(new TreeBuilder(
		"expr_4_tail",
		{ Delimiter::less },
		{ Delimiter::less, "expr_4" }
	));

	add_builder(new TreeBuilder(
		"expr_4_tail",
		{},
		{}
	));

	// expr_5 -> term { (`+`|`-`) term }

	add_builder(new TreeBuilder(
		"expr_5",
		{ expr_peek_ },
		{ "term", "expr_5_tail" }
	));

	add_builder(new TreeBuilder(
		"expr_5_tail",
		{ Delimiter::add },
		{ Delimiter::add, "expr_5" }
	));

	add_builder(new TreeBuilder(
		"expr_5_tail",
		{ Delimiter::minus },
		{ Delimiter::minus, "expr_5" }
	));

	add_builder(new TreeBuilder(
		"expr_5_tail",
		{},
		{}
	));

	// term -> factor { (`*`|`/`|`%`) factor }

	add_builder(new TreeBuilder(
		"term",
		{ expr_peek_ },
		{ "factor", "term_tail" }
	));

	add_builder(new TreeBuilder(
		"term_tail",
		{ Delimiter::star },
		{ Delimiter::star, "term" },
		"term_tail_star"
	));

	add_builder(new TreeBuilder(
		"term_tail",
		{ Delimiter::divide },
		{ Delimiter::divide, "term" },
		"term_tail_divide"
	));

	add_builder(new TreeBuilder(
		"term_tail",
		{ Delimiter::mod },
		{ Delimiter::mod, "term" },
		"term_tail_mod"
	));

	add_builder(new TreeBuilder(
		"term_tail",
		{},
		{},
		"term_tail_empty"
	));

	// factor -> entity | entity `++` | entity `--` | `+` entity | `-` entity

	add_builder(new TreeBuilder(
		"factor",
		{ entity_peek_ },
		{ "entity", "entity_tail_op" }
	));

	add_builder(new TreeBuilder(
		"entity_tail_op",
		{ Delimiter::increment },
		{ Delimiter::increment },
		"entity_tail_op_increment"
	));

	add_builder(new TreeBuilder(
		"entity_tail_op",
		{ Delimiter::decrement },
		{ Delimiter::decrement },
		"entity_tail_op_decrement"
	));

	add_builder(new TreeBuilder(
		"entity_tail_op",
		{},
		{}
	));

	add_builder(new TreeBuilder(
		"factor",
		{ Delimiter::add },
		{ Delimiter::add, "entity" }
	));

	add_builder(new TreeBuilder(
		"factor",
		{ Delimiter::minus },
		{ Delimiter::minus, "entity" }
	));

}

void TreeMeta::add_statement_defs()
{
	// TODO : test the following

	add_builder(new TreeBuilder(
		"statement",
		{ entity_peek_ },
		{ "entity", "statement_right" }
	));

	add_builder(new TreeBuilder(
		"statement_right",
		{ Delimiter::assign },
		{ Delimiter::assign, "expr" }
	));

	add_builder(new TreeBuilder(
		"statement",
		{ Word::keyword_return },
		{ Word::keyword_return, "expr" }
	));

	add_builder(new TreeBuilder(
		"statement_block",
		{ entity_peek_ },
		{ "statement", "statement_block_tail" }
	));

	add_builder(new TreeBuilder(
		"statement_block",
		{ Word::keyword_return },
		{ "statement", "statement_block_tail" }
	));

	add_builder(new TreeBuilder(
		"statement_block_tail",
		{ entity_peek_ },
		{ "statement_block" }
	));

	add_builder(new TreeBuilder(
		"statement_block_tail",
		{ Word::keyword_return },
		{ "statement_block" }
	));

	add_builder(new TreeBuilder(
		"statement_block_tail",
		{},
		{}
	));
}

void TreeMeta::add_func_defs()
{
	add_builder(new TreeBuilder(
		"func_def_and_optional_call",
		{ Word::keyword_fn },
		{ Word::keyword_fn, "comma_identifiers", "func_body", "optional_call" }
	));

	add_builder(new TreeBuilder(
		"comma_identifiers",
		{ Word::identifier },
		{ Word::identifier, "comma_identifiers_tail" }
	));

	add_builder(new TreeBuilder(
		"comma_identifiers",
		{ },
		{ }
	));

	add_builder(new TreeBuilder(
		"comma_identifiers_tail",
		{ Delimiter::comma },
		{ Delimiter::comma, "comma_identifiers" }
	));

	add_builder(new TreeBuilder(
		"comma_identifiers_tail",
		{},
		{}
	));

	add_builder(new TreeBuilder(
		"func_body",
		{ Delimiter::left_brace },
		{ Delimiter::left_brace, "statement_block", Delimiter::right_brace }
	));

	add_builder(new TreeBuilder(
		"optional_call",
		{ Delimiter::left_paren },
		{ Delimiter::left_paren, "comma_exprs", Delimiter::right_paren }
	));

	add_builder(new TreeBuilder(
		"optional_call",
		{},
		{}
	));

	add_builder(new TreeBuilder(
		"comma_exprs",
		{ expr_peek_, "comma_exprs_tail" },
		{ "expr", "comma_exprs_tail" }
	));

	add_builder(new TreeBuilder(
		"comma_exprs",
		{},
		{}
	));

	add_builder(new TreeBuilder(
		"comma_exprs_tail",
		{ Delimiter::comma },
		{ Delimiter::comma, "comma_exprs" }
	));

	add_builder(new TreeBuilder(
		"comma_exprs_tail",
		{},
		{}
	));
}

void TreeMeta::add_entity_defs()
{
	// atom -> identifier
	add_builder(new TreeBuilder(
		"atom",
		{ Word::identifier, tutils::negative(Delimiter::left_paren) },
		{ Word::identifier },
		"atom_identifier"
	));

	// atom -> func_call
	add_builder(new TreeBuilder(
		"atom",
		{ Word::identifier, Delimiter::left_paren },
		{ Word::identifier, Delimiter::left_paren, "comma_exprs", Delimiter::right_paren }
	));

	// atom -> (expr)
	add_builder(new TreeBuilder(
		"atom",
		{ Delimiter::left_paren },
		{ Delimiter::left_paren, "expr", Delimiter::right_paren }
	));

	// atom -> func_def_and_optional_call
	add_builder(new TreeBuilder(
		"atom",
		{ Word::keyword_fn },
		{ "func_def_and_optional_call" }
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
		{ tutils::logical_or({ Word::identifier, Word::keyword_fn, Delimiter::left_paren }) },
		{ "atom", "sub_entity" },
		"entity_atom_sub_entity"
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