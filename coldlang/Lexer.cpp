#include "stdafx.h"
#include "Lexer.h"
#include <cctype>

void Lexer::skip_blanks() {
	auto peek = peek_char();
	while (peek == L'\t' || peek == L' ' || peek == L'\n') {
		next_char();
		peek = peek_char();
	}
}

void Lexer::skip_comments()
{
	auto peek = peek_char();
	if (peek == L'/')
	{
		if (peek_char(1) == L'/')
		{
			next_char();
			next_char(); // found '//'
			auto peek = peek_char();
			while (peek != L'\n') { // skip until '\n'
				next_char();
				peek = peek_char();
			}
		}
		if (peek_char(1) == L'*')
		{
			next_char();
			next_char(); // found '/*'
			auto peek = peek_char();
			auto last = L'0';
			while (true) { 
				if (last == L'*' && peek == L'/')
				{
					next_char();
					break;
				}
				last = next_char();
				peek = peek_char();
			}
		}
	}

}

// @lends: Token* of current pos
shared_ptr<Token> & Lexer::next_token() {
	//wcout << "token_pointer_ = " << token_pointer_ << endl;
	//wcout << "token_list_.size() - 1 = " << (token_list_.size() - 1) << endl;
	if (int(token_pointer_) > int(token_list_.size() - 1)) {
		// wcout << "pushed" << endl;
		auto new_token = shared_ptr<Token>(parse_next_token());
		token_list_.push_back(new_token);
	}
	return token_list_.at(token_pointer_++);
}

// @lends: Token* of current pos
shared_ptr<Token> & Lexer::prev_token() {
	assert(token_pointer_ >= 1);
	return token_list_.at(token_pointer_--);
}

// @lends: Token* of (current pos + offset)
shared_ptr<Token> & Lexer::peek_token(unsigned int offset)
{
	// TODO: exception
	//wcout << "token_list_.size() - 1 = " << int(token_list_.size()) - 1 << endl;
	//wcout << "test number: " << 123 << endl;
	while (int(token_pointer_ + offset) > int(token_list_.size()) - 1)
	{

		token_list_.push_back(shared_ptr<Token>(parse_next_token()));
	}
	return token_list_.at(token_pointer_ + offset);
}

Token * Lexer::parse_next_token() {
	skip_blanks();
	skip_comments();
	skip_blanks();
	const auto peek = peek_char();
	Token * token;
	// wcout << "peek: " << peek << endl;
	if (iswalpha(peek) || peek == L'_')
	{
		token = parse_next_word();
	}
	else if (iswdigit(peek))
	{
		token = parse_next_number();
	}
	else if (peek == '\'') {
		token = parse_next_string();
	}
	else {
		token = delimiter_parser_.parse();
	}
	if (new_line_ && token != nullptr)
	{
		token->set_at_line_head(true);
		new_line_ = false;
	}
	// if (token) wcout << token->get_raw_string() << " is head: " << token->get_at_line_head() << endl;
	return token;
}

// reads current char, forwards the pointer 
wchar_t Lexer::next_char() {
	if (code_pointer_ < code_->length()) {
		if (code_->at(code_pointer_) == '\n') {
			line_++;
			col_ = 0;
			new_line_ = true;
		}
		else {
			col_++;
		}
		return code_->at(code_pointer_++);
	}
	else {
		return EOF;
	}
}

// reads current char, without side-effects
wchar_t Lexer::peek_char(int offset) {
	if (code_pointer_ + offset < code_->length()) {
		return code_->at(code_pointer_ + offset);
	}
	else {
		return EOF;
	}
}

Token * Lexer::parse_next_word() {
	ResizableBuffer<wchar_t> resizable_buffer(1024);
	wchar_t peek = peek_char();
	int line = line_;
	int col = col_;
	if (isalpha(peek)) {
		resizable_buffer.push(peek);
		next_char();
		peek = peek_char();
		while (iswalpha(peek) || iswdigit(peek) || peek == L'_') {
			resizable_buffer.push(peek);
			next_char();
			peek = peek_char();
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
	else {
		resizable_buffer.free_buf();
		// TODO
		// throw UnexpectedCharacterException()
	}
	return NULL;
}

Token * Lexer::parse_next_string() {
	int line = line_;
	int col = col_;

	ResizableBuffer<wchar_t> raw_string_buf(1024);
	wchar_t peek = next_char();
	raw_string_buf.push(peek);
	//wcout << "parse_next_string peek: " << peek << endl;

	wchar_t* escaped = string_parser_.parse(raw_string_buf, '\'');
	peek = peek_char();
	//wcout << "parse_next_string peek: " << peek << endl;
	if (peek == '\'') {
		raw_string_buf.push(peek);
		next_char();
	}
	else {
		raw_string_buf.free_buf();
		// TODO:
		// throw error
	}

	return new String(this->module_, raw_string_buf.get_null_terminated_buf(), line, col, escaped);
}

Token* Lexer::parse_next_number()
{
	if (peek_char(0) == '0' && iswalpha(peek_char(1)))
	{
		auto format = (peek_char(1) == 'b' || peek_char(1) == 'B') ? NumberParser::Binary :
			(peek_char(1) == 'x' || peek_char(1) == 'X') ? NumberParser::Hexadecimal :
			-1;
		if (format == -1)
		{
			throw new NumberFormatException("Invalid specification of format", nullptr);
		}
		next_char();
		next_char();
		int line = line_;
		int col = col_;
		ResizableBuffer<wchar_t> raw_buf(10);
		uint64_t result = integer_parser_.parseInt(static_cast<NumberParser::IntegerFormat>(format), raw_buf);
		return new Integer(module_, raw_buf.get_null_terminated_buf(), line, col, result);
	}
	else
	{
		return integer_parser_.parseNumber();
	}
}
