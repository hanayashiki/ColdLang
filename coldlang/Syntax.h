#pragma once
#include "stdafx.h"

class Syntax
{
private:
	Lexer * lexer_;
	TreeMeta * tree_meta_;
public:
	Syntax(Lexer * lexer_);
	Tree* parse();
	TreeNode* parse(vector<TreeBuilder*> & tree_builder);
	TreeNode* parse(const TreeBuilder* tree_builder);
	TreeBuilder* select_tree_builder(vector<TreeBuilder*> & v) const;
	~Syntax();
};