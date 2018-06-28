#include "stdafx.h"

DelimiterParser::DelimiterParser(Lexer* lexer) : lexer_(lexer) {
	root = new DelimiterParseState();
	root->add_state('+',
		(new DelimiterParseState())->add_state('+',
			new DelimiterParseState(Delimiter::increment)				// "++"
		)->add_state('\0', new DelimiterParseState(Delimiter::add))		// "+"
	);
}

Delimiter * DelimiterParser::parse() {
	int line = lexer_->line_;
	int col = lexer_->col_;
	DelimiterParseState* state = root;
	ResizableBuffer<wchar_t> buf_(3);
	while (true) {
		bool accept, bad, move;
		wchar_t peek = lexer_->peek_char();
		DelimiterParseState* next_state = state->get_next_state(peek, accept, bad, move);
		if (accept) {
			return new Delimiter(lexer_->module_, buf_.get_null_terminated_buf(), line, col, state->get_result_tag_());
		}
		if (move) {
			buf_.push(peek);
			lexer_->next_char();
		}
		if (bad) {
			break;
			// TODO: throw something
		}
		state = next_state;
	}
	return NULL;
}