#pragma once
#include "stdafx.h"

class Syntax
{
private:
	Lexer * lexer_;
	TreeMeta * tree_meta_;
public:
	Syntax(Lexer * lexer_, TreeMeta * tree_meta);

};