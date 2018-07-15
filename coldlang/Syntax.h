#pragma once
#include "stdafx.h"

class Syntax
{
private:
	Lexer * lexer_;
	TreeMeta * tree_meta_;

	bool bad_syntax_flag_;
public:
	Syntax(Lexer * lexer_);
	Tree* parse(const char * root_name = "expr");
	TreeNode* parse(vector<TreeBuilder*> & tree_builder);
	TreeNode* parse(const TreeBuilder* tree_builder);
	TreeBuilder* select_tree_builder(vector<TreeBuilder*> & v);
	bool judge_peek_list(const vector<TreeUnitBuilder> & peek_list);
	~Syntax();
};