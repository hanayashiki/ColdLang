#pragma once
#include "stdafx.h"

class Lexer;

class NumberParser {
private:
	Lexer * lexer_;
	enum FloatState {
		Whole,
		Fraction,
		Exponential
	};
public:
	enum IntegerFormat {
		Binary,			// 1, 0, allowing '_'
		Decimal,		// 0-9, allowing '\''
		Hexadecimal		// 0-9, a-f or A-F, allowing '_'
	};
	NumberParser(Lexer* lexer) : lexer_(lexer) {}
	uint64_t parseInt(IntegerFormat format, ResizableBuffer<wchar_t> & raw_buf, int digit_min = 0, int digit_limit = -1) const;
	Token* parseNumber() const;
};