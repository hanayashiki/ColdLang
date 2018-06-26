#include "stdafx.h"
#include "Lexer.h"
#include <cctype>

Token* Lexer::nextToken() {
	assert(false);
	return NULL;
}

Token* Lexer::prevToken() {
	assert(false);
	return NULL;
}

Token * Lexer::parseNextToken() {
	skip_blanks();
	auto peek = peekChar();
	if (isalpha(peek))
	{
		return parseNextWord();
	} else if (isdigit(peek))
	{
		return NULL;
	}
	else {
		return NULL;
	}

}

void Lexer::skip_blanks() {
	auto peek = peekChar();
	if (peek == L'\t' || peek == L' ' || peek == L'\n') {
		peek = nextChar();
	}
}

// reads current char, forwards the pointer 
wchar_t Lexer::nextChar() {
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
wchar_t Lexer::peekChar() {
	if (code_pointer_ < code_->length()) {
		return code_->at(code_pointer_);
	}
	else {
		return EOF;
	}
}

Token * Lexer::parseNextWord() {
	ResizableBuffer<wchar_t> resizable_buffer(1024);
	wchar_t peek = nextChar();
	int line = line_;
	int col = col_;
	if (isalpha(peek)) {
		resizable_buffer.push(peek);
		peek = nextChar();
		while (isalpha(peek) || isdigit(peek)) {
			resizable_buffer.push(peek);
			peek = nextChar();
		}
		return new Token(module_, resizable_buffer.get_null_terminated_buf(), line, col);
	}
	return NULL;
}