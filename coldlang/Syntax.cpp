#include "stdafx.h"

Syntax::Syntax(Lexer* lexer, TreeMeta * tree_meta): lexer_(lexer), tree_meta_(tree_meta)
{
	// entity -> identifier
	tree_meta_->add_builder(new TreeBuilder(
		"entity",
		{ Word::identifier, tutils::negative(Delimiter::left_paren) },
		{ Word::identifier }
	));
	// entity -> function_call
	tree_meta_->add_builder(new TreeBuilder(
		"entity",
		{ Word::identifier, Delimiter::left_paren },
		{ Word::identifier, Delimiter::left_paren, Delimiter::right_paren }
	));
}

