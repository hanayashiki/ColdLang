#include "stdafx.h"

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
			buf_.free_buf();
			break;
			// TODO: throw something
		}
		state = next_state;
	}
	return nullptr;
}

DelimiterParser::DelimiterParser(Lexer* lexer) : lexer_(lexer) {
	root = new DelimiterParseState();
	root->add_state('+',
		((new DelimiterParseState())
			->add_state('+',
				new DelimiterParseState(Delimiter::increment))					// "++"
			->add_state('=',
				new DelimiterParseState(Delimiter::add_assign))					// "+="
			)->add_state('\0', new DelimiterParseState(Delimiter::add))			// "+"
	);

	root->add_state('-',
		((new DelimiterParseState())
			->add_state('-',
				new DelimiterParseState(Delimiter::decrement))					// "--"		
			->add_state('=',
				new DelimiterParseState(Delimiter::sub_assign))					// "-="
			)->add_state('\0', new DelimiterParseState(Delimiter::minus))		// "-"
	);

	root->add_state('*',
		((new DelimiterParseState())
			->add_state('=',
				new DelimiterParseState(Delimiter::mult_assign))				// "*="
			)->add_state('\0', new DelimiterParseState(Delimiter::star))		// "*"
	);

	root->add_state('/',
		((new DelimiterParseState())
			->add_state('=',
				new DelimiterParseState(Delimiter::div_assign))					// "/="
			)->add_state('\0', new DelimiterParseState(Delimiter::divide))		// "/"
	);

	root->add_state('=',
		((new DelimiterParseState())
			->add_state('=', new DelimiterParseState(Delimiter::equal)))		// "=="
		->add_state('\0', new DelimiterParseState(Delimiter::assign))			// "="
	);

	root->add_state('%',
		((new DelimiterParseState())
			->add_state('=', new DelimiterParseState(Delimiter::mod_assign)))	// "%="
		->add_state('\0', new DelimiterParseState(Delimiter::mod))				// "%"
	);

	root->add_state('<',
		((new DelimiterParseState())
			->add_state('=', new DelimiterParseState(Delimiter::less_equal)))	// "<"
		->add_state('\0', new DelimiterParseState(Delimiter::less))				// "<="
	);

	root->add_state('>',
		((new DelimiterParseState())
			->add_state('=', new DelimiterParseState(Delimiter::greater_equal)))	// ">"
		->add_state('\0', new DelimiterParseState(Delimiter::greater))				// ">="
	);

	root->add_state('!',
		((new DelimiterParseState())
			->add_state('=', new DelimiterParseState(Delimiter::not_equal)))	// "!="
		->add_state('\0', new DelimiterParseState(Delimiter::_not))				// "!"
	);

	root->add_state('&',
		((new DelimiterParseState())
			->add_state('&', new DelimiterParseState(Delimiter::_and)))			// "&&"
	);

	root->add_state('|',
		((new DelimiterParseState())
			->add_state('|', new DelimiterParseState(Delimiter::_or)))			// "||"
	);

	root->add_state('.',
		(new DelimiterParseState(DelimiterParseState(Delimiter::period)))		// "."
	);

	root->add_state(',',
		(new DelimiterParseState(DelimiterParseState(Delimiter::comma)))		// ","
	);

	root->add_state(':',
		(new DelimiterParseState(DelimiterParseState(Delimiter::colon)))		// ":"
	);

	root->add_state('?',
		(new DelimiterParseState(DelimiterParseState(Delimiter::question)))		// "?"
	);

	root->add_state('(',
		(new DelimiterParseState(DelimiterParseState(Delimiter::left_paren)))	// "("
	);

	root->add_state(')',
		(new DelimiterParseState(DelimiterParseState(Delimiter::right_paren)))	// ")"
	);

	root->add_state('[',
		(new DelimiterParseState(DelimiterParseState(Delimiter::left_bracket)))	// "["
	);

	root->add_state(']',
		(new DelimiterParseState(DelimiterParseState(Delimiter::right_bracket)))	// "]"
	);

	root->add_state('{',
		(new DelimiterParseState(DelimiterParseState(Delimiter::left_brace)))	// "{"
	);

	root->add_state('}',
		(new DelimiterParseState(DelimiterParseState(Delimiter::right_brace)))	// "}"
	);
}