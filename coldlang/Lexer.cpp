#include "stdafx.h"
#include "Lexer.h"
#include <cctype>

Token* Lexer::next_token() {
	assert(false);
	return NULL;
}

Token* Lexer::prev_token() {
	assert(false);
	return NULL;
}

Token * Lexer::parse_next_token() {
	skip_blanks();
	auto peek = peek_char();
	if (isalpha(peek) || peek == L'_')
	{
		return parse_next_word();
	} else if (isdigit(peek))
	{
		return NULL;
	}
	else {
		return NULL;
	}

}

void Lexer::skip_blanks() {
	auto peek = peek_char();
	if (peek == L'\t' || peek == L' ' || peek == L'\n') {
		peek = next_char();
	}
}

// reads current char, forwards the pointer 
wchar_t Lexer::next_char() {
	if (code_pointer_ < code_->length()) {
		if (code_->at(code_pointer_) == '\n') {
			line_++;
			col_ = 0;
		}
		else {
			col_ ++;
		}
		return code_->at(code_pointer_++);
	}
	else {
		return EOF;
	}
}

// reads current char, without side-effects
wchar_t Lexer::peek_char() {
	if (code_pointer_ < code_->length()) {
		return code_->at(code_pointer_);
	}
	else {
		return EOF;
	}
}

Token * Lexer::parse_next_word() {
	ResizableBuffer<wchar_t> resizable_buffer(1024);
	wchar_t peek = next_char();
	int line = line_;
	int col = col_;
	if (isalpha(peek)) {
		resizable_buffer.push(peek);
		peek = next_char();
		while (isalpha(peek) || isdigit(peek) || peek == L'_') {
			resizable_buffer.push(peek);
			peek = next_char();
		}
		wchar_t* new_word = resizable_buffer.get_null_terminated_buf();
		Word::WordType type = (Word::WordType)keyword_trie_.get_tag(new_word, resizable_buffer.get_ptr() - 1);
		if (type != -1) {
			return new Word(module_, new_word, line, col, type);
		}
		else {
			return new Word(module_, new_word, line, col, Word::identifier);
		}
	}
	return NULL;
}