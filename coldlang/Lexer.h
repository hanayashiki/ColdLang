#pragma once
#include "stdafx.h"
#include "KeywordTrie.h"

class Lexer {
private:
	int line_;
	int col_;

	const std::wstring * code_;
	std::vector<Token*> token_list;
	Module* module_;
	KeywordTrie keyword_trie_;

	IntegerParser integer_parser_;
	StringParser string_parser_;
	DelimiterParser delimiter_parser_;

	unsigned int code_pointer_;
	void skip_blanks();
	wchar_t next_char();
	wchar_t peek_char();
public:
	// @requires: use new, or direct constructor
	Lexer(Module* _module) :
		module_(_module),
		code_(&module_->code),
		integer_parser_(IntegerParser(this)),
		string_parser_(StringParser(this)),
		delimiter_parser_(DelimiterParser(this))
	{
		line_ = 1;
		col_ = 1;
		code_pointer_ = 0;
	};
	// @requires: use new, or direct constructor
	Lexer(std::wstring* code) :
		module_(NULL),
		code_(code),
		integer_parser_(IntegerParser(this)),
		string_parser_(StringParser(this)),
		delimiter_parser_(DelimiterParser(this))
	{
		line_ = 1;
		col_ = 1;
		code_pointer_ = 0;
	};
	// @lends: Token* result
	Token * next_token();
	// @lends: Token* result
	Token * prev_token();
	Token * parse_next_token();
	Token * parse_next_word();
	Token * parse_next_string();

	friend class StringParser;
	friend class IntegerParser;
	friend class DelimiterParser;

	~Lexer() {
		for (auto t : token_list) {
			delete(t);
		}
	}
};