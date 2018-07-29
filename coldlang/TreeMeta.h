#pragma once
#include "stdafx.h"
#include <unordered_map>

class TreeMeta
{
private:
	std::unordered_map<string, std::vector<TreeBuilder*>> name_to_builder_;
	const TreeUnitBuilder entity_peek_ = tutils::logical_or({
		Word::identifier,
		String::GeneralString,
		"integer",
		"float",
		Word::keyword_fn,
		Word::keyword_true,
		Word::keyword_false,
		Word::keyword_none,
		Delimiter::left_paren
	});
	const TreeUnitBuilder expr_peek_ = tutils::logical_or({
		Word::identifier,
		String::GeneralString,
		"integer",
		"float",
		Word::keyword_fn,
		Word::keyword_true,
		Word::keyword_false,
		Word::keyword_none,
		Delimiter::left_paren,
		Delimiter::add,
		Delimiter::minus
	});
	const TreeUnitBuilder statement_peek_ = tutils::logical_or({
		Word::identifier,
		Word::keyword_if,
		Word::keyword_return,
		Word::keyword_return_value,
		Word::keyword_while,
		Word::keyword_break,
		String::GeneralString,
		"integer",
		"float",
		Word::keyword_fn,
		Delimiter::left_paren,
	});


public:
	TreeMeta();
	void add_builder(TreeBuilder*);
	vector<TreeBuilder*> & get_tree_builders(char* name);
	vector<TreeBuilder*> & get_tree_builders(string name);
	void add_entity_defs();
	void add_expr_defs();
	void add_statement_defs();
	void add_func_defs();
	~TreeMeta();
};
