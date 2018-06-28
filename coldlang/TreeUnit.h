#pragma once
#include "stdafx.h"
#include "Syntax.h"

class TreeBuilder;

class TreeUnit : public TreeNode {
public:
	enum UnitType {
		Delimiter,
		Float,
		Integer,
		String,
		Token,
		Word,
		TreeBuilder
	};
	TreeUnit(const char* n);
	TreeUnit(Word::WordType word_type);
	TreeUnit(Delimiter::DelimiterType delimiter_type);
	TreeUnit(String::StringType string_type);
	TreeUnit(TreeNode * tb);
};
