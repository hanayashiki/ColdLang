#include "stdafx.h"

wchar_t* StringParser::parse(ResizableBuffer<wchar_t> & raw_buf, wchar_t delimiter) {
	ResizableBuffer<wchar_t> buf(1024);
	wchar_t wc = lexer_->peek_char();
	while (wc != delimiter) {
		wc = lexer_->next_char();
		raw_buf.push(wc);
		if (wc == '\\') {
			wc = lexer_->next_char();
			raw_buf.push(wc);
			switch (wc) {
			case '\\':
				buf.push('\\');
				break;
			case '\'':
				buf.push('\'');
				break;
			case '\"':
				buf.push('\"');
				break;
			case 'a':
				buf.push('\a');
				break;
			case 'b':
				buf.push('\b');
				break;
			case 'n':
				buf.push('\n');
				break;
			case 'v':
				buf.push('\v');
				break;
			case 't':
				buf.push('\t');
				break;
			case 'f':
				buf.push('\f');
				break;
			case 'u':
				unicode_parser(buf, 4);
				break;
			case 'U':
				unicode_parser(buf, 8);
				break;
			case 'x':
				unicode_parser(buf, 2);
				break;
			default:
				// TODO
				// throw something
				break;
			}
		}
		else if (wc != '\n') {
			buf.push(wc);
		}
		else {
			// TODO
			// throw something
			buf.free_buf();
		}
		wc = lexer_->peek_char();
	}
	return buf.get_null_terminated_buf();
}

// unicode -> utf-16: https://zh.wikipedia.org/wiki/UTF-16
void StringParser::unicode_parser(ResizableBuffer<wchar_t> & buf, int length) {
	int64_t unicode = lexer_->integer_parser_.parseInt(IntegerParser::Hexadecimal, length, length);
	if (unicode >= 0x0 && unicode <= 0xd7ff) {
		buf.push((wchar_t)unicode);
	}
	else if (unicode >= 0xe000 && unicode <= 0xffff) {
		buf.push((wchar_t)unicode);
	}
	else if (unicode >= 0x10000 && unicode <= 0x10ffff) {
		int64_t top10 = ((unicode - 0x10000) & (~0x3ff)) >> 10;
		int64_t low10 = ((unicode - 0x10000) & 0x3ff);
		buf.push((wchar_t)(top10 | 0xd800));
		//wcout << hex << (top10 | 0xd800) << dec << endl; 
		buf.push((wchar_t)(low10 | 0xdc00));
		//wcout << hex << (low10 | 0xdc00) << dec << endl;
	}
	else {
		// TODO:
		// throw unicode error
	}
}