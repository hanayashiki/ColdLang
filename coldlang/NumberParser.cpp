#include "stdafx.h"
#include "NumberParser.h"

uint64_t NumberParser::parseInt(IntegerFormat format, ResizableBuffer<wchar_t> & raw_buf, int min_length, int digit_limit) const
{
	uint64_t sum = 0;
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
			if (iswdigit(peek)) {
				legal = true;
				sum = 10 * sum + (peek - '0');
			}
			else if (peek == '\'') {
				legal = true;
				i--;
			}
		}
		else if (format == IntegerFormat::Hexadecimal) {
			if (iswdigit(peek)) {
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
			// wcout << "IntegerParser::parseInt as legal: " << peek << endl;
			got_int = true;
			raw_buf.push(peek);
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

Token* NumberParser::parseNumber() const
{
	auto raw_buf = new ResizableBuffer<wchar_t>(10);
	FloatState state = Whole;
	uint64_t uint_val = 0;
	const uint64_t max_uint_val = 0x7FFFFFFFFFFFFFFFULL + 1ULL;
	int line = lexer_->line_;
	int col = lexer_->col_;
	while (true) {
		wchar_t peek = lexer_->peek_char();
		switch (state) {
		case Whole:
			if (iswdigit(peek)) {
				raw_buf->push(peek);
				bool overflow = false;
				// max_uint_val = 9,223,372,036,854,775,807
				if (uint_val == max_uint_val / 10) {
					if (peek - '0' > 8)
					{
						overflow = true;
					}
				}
				else if (uint_val > max_uint_val / 10)
				{
					overflow = true;
				}
				if (overflow)
				{
					Integer * token = new Integer(lexer_->module_, raw_buf->get_null_terminated_buf(), line, col, uint_val);
					throw new NumberFormatException("Integer token cannot be greater than 2**63 - 1", token);
				}
				uint_val = 10 * uint_val + (peek - '0');
				lexer_->next_char();
			}
			else if (peek == L'.') {
				state = Fraction;
				raw_buf->push(peek);
				lexer_->next_char();
			}
			else if (peek == L'e' || peek == L'E')
			{
				state = Exponential;
				raw_buf->push(peek);
				lexer_->next_char();
			}
			else
			{
				return new Integer(lexer_->module_, raw_buf->get_null_terminated_buf(), line, col, uint_val);
			}
			break;
		case Fraction:
			if (iswdigit(peek)) {
				raw_buf->push(peek);
				lexer_->next_char();
			}
			else if (peek == L'e' || peek == L'E')
			{
				state = Exponential;
				raw_buf->push(peek);
				lexer_->next_char();
			}
			else
			{
				const double float_val = wcstod(raw_buf->get_null_terminated_buf(), nullptr);
				return new Float(lexer_->module_, raw_buf->get_null_terminated_buf(), line, col, float_val);
			}
			break;
		case Exponential:
			if (iswdigit(peek)) {
				raw_buf->push(peek);
				lexer_->next_char();
			}
			else
			{
				const double float_val = wcstod(raw_buf->get_null_terminated_buf(), nullptr);
				return new Float(lexer_->module_, raw_buf->get_null_terminated_buf(), line, col, float_val);
			}
			break;
		}
	}
	return nullptr;
}
