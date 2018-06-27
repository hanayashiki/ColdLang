#pragma once
#include "stdafx.h"

class Lexer;

class StringParser {
private:
	Lexer* lexer_;
public:
	// @borrows: raw_string
	StringParser(Lexer* lexer):
		lexer_(lexer)
	{

	}

	// @offers: buf_.get_null_terminated_buf()
	// escape		effect
	//	\\			escape \
	//	\'			escape '
	//	\"			escape "
	//	\a			to \a
	//	\b			to \b
	//	\n			to \n
	//	\v			to \v
	//	\t			to \t
	//	\f			to \f
	//	\u(x{1,8})	to unicode£¬ \u \U \x \X supported
	//	
	wchar_t* parse(ResizableBuffer<wchar_t> & raw_buf, wchar_t delimiter = '\'');

	void unicode_parser(ResizableBuffer<wchar_t> &, int length);

};