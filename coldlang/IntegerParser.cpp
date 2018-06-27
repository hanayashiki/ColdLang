#include "stdafx.h"

int64_t IntegerParser::parseInt(IntegerFormat format, int min_length, int digit_limit) { 
	int64_t sum = 0;
	bool got_int = false;
	for (int i = 0; digit_limit < 0 || i < digit_limit; i++) {
		wchar_t peek = lexer_->peek_char();
		bool legal = false;
		if (format == IntegerFormat::Binary) {
			if (peek == '0' || peek == '1') {
				legal = true;
				sum = 2 * sum + (peek - '0');
			}
			else if (peek == '_') {
				legal = true;
				i--;
			}
		}
		else if (format == IntegerFormat::Decimal) {
			if (isdigit(peek)) {
				legal = true;
				sum = 10 * sum + (peek - '0');
			} 
			else if (peek == '\'') {
				legal = true;
				i--;
			}
		}
		else if (format == IntegerFormat::Hexadecimal) {
			if (isdigit(peek)) {
				legal = true;
				sum = 16 * sum + (peek - '0');
			}
			else if (peek >= 'a' && peek <= 'f') {
				legal = true;
				sum = 16 * sum + (10 + peek - 'a');
			}
			else if (peek >= 'A' && peek <= 'F') {
				legal = true;
				sum = 16 * sum + (10 + peek - 'A');
			}
			else if (peek == '_') {
				legal = true;
				i--;
			}
		}
		if (legal) {
			wcout << "IntegerParser::parseInt as legal: " << peek << endl;
			got_int = true;
			lexer_->next_char();
		}
		else if (got_int) {
			return sum;
		}
		else {
			// TODO: throw something
		}
	}
	return sum;
}