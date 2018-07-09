#pragma once
#include "stdafx.h"
#include "KeywordTrie.h"

class Lexer {
private:
	int line_;
	int col_;

	const std::wstring * code_;
	std::vector<Token*> token_list_;
	unsigned int token_pointer_;

	Module* module_;
	KeywordTrie keyword_trie_;

	NumberParser integer_parser_;
	StringParser string_parser_;
	DelimiterParser delimiter_parser_;

	unsigned int code_pointer_;
	void skip_blanks();
	wchar_t next_char();
	wchar_t peek_char(int offset = 0);
public:
	// @requires: use new, or direct constructor
	Lexer(Module* _module) :
		module_(_module),
		code_(&module_->code),
		integer_parser_(NumberParser(this)),
		string_parser_(StringParser(this)),
		delimiter_parser_(DelimiterParser(this))
	{
		line_ = 1;
		col_ = 1;
		code_pointer_ = 0;
		token_pointer_ = 0;
	};
	// @requires: use new, or direct constructor
	Lexer(std::wstring* code) :
		module_(NULL),
		code_(code),
		integer_parser_(NumberParser(this)),
		string_parser_(StringParser(this)),
		delimiter_parser_(DelimiterParser(this))
	{
		line_ = 1;
		col_ = 1;
		code_pointer_ = 0;
		token_pointer_ = 0;
	};
	// @lends: Token* result
	Token * next_token();
	// @lends: Token* result
	Token * prev_token();
	// @lends: Token* result
	Token * peek_token(unsigned int offset);
	Token * parse_next_token();
	Token * parse_next_word();
	Token * parse_next_string();
	Token * parse_next_number();

	friend class StringParser;
	friend class NumberParser;
	friend class DelimiterParser;

	~Lexer() {
		for (auto t : token_list_) {
			wcout << "deleting: " << (t != nullptr && t->get_raw_string() != nullptr ? t->get_raw_string() : L"unknown") << endl;
			delete t;
		}
	}
};