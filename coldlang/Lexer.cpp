#include "stdafx.h"
#include "Lexer.h"

Token* Lexer::nextToken() {
	assert(false);
	return NULL;
}

Token* Lexer::prevToken() {
	assert(false);
	return NULL;
}

Token * Lexer::parseNextToken() {
	
}

void Lexer::skip_blanks() {
	wchar_t peek = peekChar();
	if (peek == L'\t' || peek == L' ' || peek == L'\n') {
		peek = nextChar();
	}
}

wchar_t Lexer::nextChar() {
	if (code_pointer < code->length) {
		return code->at(code_pointer++);
	}
	else {
		return EOF;
	}
}

wchar_t Lexer::peekChar() {
	if (code_pointer < code->length) {
		return code->at(code_pointer);
	}
	else {
		return EOF;
	}
}