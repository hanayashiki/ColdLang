#pragma once
#include "stdafx.h"

class Lexer;

class IntegerParser {
private:
	Lexer * lexer_;
public:
	enum IntegerFormat{
		Binary,			// 1, 0, allowing '_'
		Decimal,		// 0-9, allowing '\''
		Hexadecimal		// 0-9, a-f or A-F, allowing '_'
	};
	IntegerParser(Lexer* lexer) : lexer_(lexer) {}
	int64_t parseInt(IntegerFormat format, int digit_min = 0, int digit_limit = -1);
};