#include "stdafx.h"
#include "IRGenerator.h"
#include "BytecodeClass.h"
#include "RuntimeObject.h"
#include "NativeFunctions.h"
#include "NativeSymbol.h"
#include "NativeFunction.h"


/* IR Generation Convention
*
* 1. Every time <node_name>_reader is called,
*    `Acc` is allocated as resource, which means
*    it can modify `Acc` for the ONLY purpose of  runtime
*    calculation.
*    It returns either `nullptr` or `Symbol *`
*    if `nullptr` is returned, it means returning value is stored
*    in `Acc`, else `Symbol *` is returned, which is a `virtual`
*    returning value. Whether to load it to `Acc` is up to upper callers.
*
*    For example, factor -> entity -> identifier -> `my_var`
*
*    identifier reads `my_var`, it does not need any runtime calculation
*    so we simply returns `Symbol *`.
*
*    However when deduction is: expr_5 -> a*b + c*d
*    we get:
*    ```
*    LoadSymbolToAcc a
*    Mul b
*    ```
*    for a*b, returning `nullptr`
*    expr_5_reader should choose to store the value to, say, temporary
*    variable `%t0`
*    ```
*    StoreAcc %t0
*    ```
*    we get:
*    ```
*    LoadSymbolToAcc c
*    Mul d
*    ```
*    Also returning `nullptr`
*
*    expr_5_reader choose to do the add, so:
*    ```
*    StoreAcc %t1
*    LoadSymbolToAcc %t0 (freeing %t0)
*    Add %t1 (freeing %t1)
*    ```
*    expr_5_reader should return `nullptr`
*
*/


namespace IR {
	using namespace OperandType;
	using namespace BytecodeClass;

	IRGenerator::IRGenerator(SymbolTable * symbol_table,
		FunctionTable * function_table,
		LiteralTable * literal_table,
		ConstantTable * constant_table,
		BytecodeWriter * bytecode_writer)
		: symbol_table_(symbol_table), 
		function_table_(function_table),
		constant_table_(constant_table),
		literal_table_(literal_table),
		temp_table_(new TempTable(symbol_table_)),
		bytecode_writer_(bytecode_writer)
	{
		initialize_native_symbols();
	}

	void IRGenerator::initialize_native_symbols()
	{
		vector<tuple<const wchar_t*, void*>> native_functions;
		decltype(native_functions)::value_type a;
		CldRuntime::add_native_functions(native_functions);

		for (auto t : native_functions)
		{
			NativeSymbol * native = new NativeFunction(std::get<0>(t), std::get<1>(t));
			symbol_table_->add(native);
		}
		
	}

	Symbol * IRGenerator::look_up_name(Token * token)
	{
		return static_cast<Symbol*>(
			symbol_table_->get_by_name(static_cast<Word*>(token)->get_word())
			);
	}

	Symbol * IRGenerator::self_or_store(Symbol * symbol)
	{
		if (symbol == Symbol::Acc) {
			symbol = temp_table_->lend();
			EMIT(StoreAcc, bytecode_writer_, static_cast<Variable*>(symbol));
		}
		return symbol;
	}

	void IRGenerator::load_if_not_nullptr(Symbol * symbol)
	{
		if (symbol != Symbol::Acc)
		{
			emit<LoadToAcc>(bytecode_writer_, symbol);
		}
	}

	Symbol * IRGenerator::atom_reader(TreeNode * tn, bool left)
	{
		if (tn->get_builder_name() == "atom_identifier")
		{
			if (left) {
				// TODO : ref
				auto symbol = look_up_name(tn->get_terminal(0).get());
				if (symbol == nullptr)
				{
					Variable * new_var = new Variable(tn->get_terminal(0));
					symbol_table_->add(new_var);
					return new_var;
				}
				return symbol;
			}
			else
			{
				auto symbol = look_up_name(tn->get_terminal(0).get());
				if (symbol == nullptr)
				{
					throw SymbolException("Symbol not found: ", tn->get_terminal(0));
				}
				return symbol;
			}
		}
		if (tn->get_builder_name() == "atom_func_def_and_optional_call")
		{
			return func_def_and_optional_call_reader(tn->get_non_terminal(0));
		}
		if (tn->get_builder_name() == "atom_true")
		{
			return literal_table_->get_true_constant();
		}
		if (tn->get_builder_name() == "atom_false")
		{
			return literal_table_->get_false_constant();
		}
		if (tn->get_builder_name() == "atom_none")
		{
			return literal_table_->get_none_constant();
		}
		if (tn->get_builder_name() == "atom_integer")
		{
			auto integer_token = std::static_pointer_cast<Integer>(tn->get_terminal(0));
			auto rto = new CldRuntime::IntegerValue(integer_token->get_value());
			Constant * constant = new Constant(tn->get_terminal(0), rto);
			// wcout << "Constant*: " << constant << endl;
			literal_table_->add(constant);
			return constant;
		}
		if (tn->get_builder_name() == "atom_general_string")
		{
			CldRuntime::StringObject * rto = new CldRuntime::StringObject;
			auto token = tn->get_terminal(0);
			String * string = static_cast<String*>(token.get());

			rto->type = CldRuntime::StringObj;
			rto->length = wcslen(string->get_value());
			wchar_t * copied = new wchar_t[rto->length + 1];
			wcscpy_s(copied, rto->length + 1, string->get_value());
			rto->content = copied;
			constant_table_->add(rto);

			return add_literal<String>(tn->get_terminal(0), rto);
		}
		if (tn->get_builder_name() == "atom_func_call")
		{
			auto func_symbol = look_up_name(tn->get_terminal(0).get());
			vector<Symbol*> arguments = comma_exprs_reader(tn->get_non_terminal(2));
			for (auto argument : arguments) {
				if (dynamic_cast<Variable*>(argument))
				{
					EMIT(PushParamVariable, bytecode_writer_, dynamic_cast<Variable*>(argument));
				} else if(dynamic_cast<Literal*>(argument)) 
				{
					EMIT(PushParamLiteral, bytecode_writer_, dynamic_cast<Literal*>(argument));
				} else
				{
					assert(false);
				}
				if (argument->is_temp())
				{
					temp_table_->revert(static_cast<Variable*>(argument));
				}
			}
			if (auto native_func = dynamic_cast<NativeFunction*>(func_symbol))
			{
				EMIT(CallNative, bytecode_writer_, native_func);
			}
			else if (auto var = dynamic_cast<Variable*>(func_symbol)) {
				EMIT(CallFunc, bytecode_writer_, var);
			}
			/* result in Acc */
			return Symbol::Acc;
		}
		if (tn->get_builder_name() == "atom_expr")
		{
			return expr_reader(tn->get_non_terminal(1));
		}
		assert(false);
		return nullptr;
	}

	vector<Symbol*> IRGenerator::comma_exprs_reader(TreeNode * tn)
	{
		vector<Symbol*> comma_exprs;
		if (tn->get_builder_name() == "comma_exprs_empty")
		{
			return comma_exprs;
		}
		if (tn->get_builder_name() == "comma_exprs_expr_comma_exprs_tail")
		{
			TreeNode * tail;
			while (true)
			{
				TreeNode * expr = tn->get_non_terminal(0);
				Symbol * symbol = expr_reader(expr);
				symbol = self_or_store(symbol);
				comma_exprs.push_back(symbol);
				tail = tn->get_non_terminal(1);
				if (tail->get_builder_name() == "comma_exprs_tail_comma_comma_exprs")
				{
					tn = tail->get_non_terminal(1);
					if (tn->get_builder_name() == "comma_exprs_empty")
					{
						// TODO: throw something properly
						assert(false);
					}
				} else
				{
					break;
				}
			}
			return comma_exprs;
		}
		assert(false);
	}

	template<typename RealTokenType>
	Literal * IRGenerator::add_literal(shared_ptr<Token> & token, CldRuntime::RuntimeObject * rto)
	{
		shared_ptr<RealTokenType> integer = 
			dynamic_pointer_cast<RealTokenType>(token);
		Literal * constant_literal = new Literal(token, rto);
		literal_table_->add(constant_literal);
		return constant_literal;
	}

	Symbol* IRGenerator::entity_reader(TreeNode* tn, bool create)
	{
		if (tn->get_builder_name() == "entity_atom_sub_entity")
		{
			// TODO: for the chain
			return atom_reader(tn->get_non_terminal(0), create);
		}
		assert(false);
		return nullptr;
	}

	Symbol * IRGenerator::factor_reader(TreeNode * tn)
	{
		if (tn->get_builder_name() == "entity_entity_tail_op")
		{
			Symbol * entity = entity_reader(tn->get_non_terminal(0));
			entity_tail_op_reader(tn->get_non_terminal(1), entity);
			return entity;
		}
		assert(false);
		return nullptr;
	}

	Symbol * IRGenerator::term_reader(TreeNode * tn, bool head, string && left_builder_name)
	{
		Symbol * ret = nullptr;
		Symbol * factor = factor_reader(tn->get_non_terminal(0));
		if (head)
		{
			if (tn->get_non_terminal(1)->get_builder_name() == "term_tail_empty")
			{
				return factor;
			}
			else
			{
				emit<LoadToAcc>(bytecode_writer_, factor);
			}
		}
		else
		{
			if (left_builder_name == "term_tail_star")
			{
				emit<Mul>(bytecode_writer_, factor);
			}
			else if (left_builder_name == "term_tail_divide")
			{
				emit<Div>(bytecode_writer_, factor);
			}
			else if (left_builder_name == "term_tail_mod")
			{
				emit<Mod>(bytecode_writer_, factor);
			}
		}
		term_tail_reader(tn->get_non_terminal(1));
		return ret;
	}

	Symbol * IRGenerator::expr_5_reader(TreeNode * tn, bool head, Symbol * left_symbol, string && left_builder_name)
	{
		Symbol * ret = nullptr;
		Symbol * new_left_symbol = nullptr;
		if (head)
		{
			Symbol * term = term_reader(tn->get_non_terminal(0), true, "");
			if (tn->get_non_terminal(1)->get_builder_name() == "expr_5_tail_empty")
			{
				ret = term;
				return ret;
			}
			else
			{
				if (term == Symbol::Acc)
				{
					// will be overwritten, so we should store `Acc` somewhere
					Variable * temp = temp_table_->lend();
					new_left_symbol = temp;
					EMIT(StoreAcc, bytecode_writer_, temp);
				}
				else
				{
					new_left_symbol = term;
				}
			}
		}
		else
		{
			if (left_symbol == Symbol::Acc) {
				left_symbol = temp_table_->lend();
				EMIT(StoreAcc, bytecode_writer_, static_cast<Variable*>(left_symbol));
			}
			Symbol * term = term_reader(tn->get_non_terminal(0), true, "");
			if (term == Symbol::Acc)
			{
				term = temp_table_->lend();
				EMIT(StoreAcc, bytecode_writer_, static_cast<Variable*>(term));
				temp_table_->revert(static_cast<Variable*>(term));
			}
			emit<LoadToAcc>(bytecode_writer_, left_symbol);
			if (left_symbol->is_temp())
			{
				temp_table_->revert(static_cast<Variable*>(left_symbol));
			}
			if (left_builder_name == "expr_5_tail_add")
				emit<Add>(bytecode_writer_, term);
			if (left_builder_name == "expr_5_tail_minus")
				emit<Sub>(bytecode_writer_, term);
			new_left_symbol = Symbol::Acc;
		}
		expr_5_tail_reader(tn->get_non_terminal(1), new_left_symbol);
		return ret;
	}

	void IRGenerator::expr_5_tail_reader(TreeNode * tn, Symbol * target_symbol)
	{
		if (tn->get_builder_name() != "expr_5_tail_empty") {
			expr_5_reader(tn->get_non_terminal(1), false, target_symbol, tn->get_builder_name());
		}
	}

	void IRGenerator::term_tail_reader(TreeNode * tn)
	{
		if (tn->get_builder_name() != "term_tail_empty") {
			term_reader(tn->get_non_terminal(1), false, tn->get_builder_name());
		}
	}

	void IRGenerator::entity_tail_op_reader(TreeNode * tn, Symbol * target_symbol)
	{
		if (tn->get_builder_name() == "entity_tail_op_increment")
		{
			side_effect_stack.top().push_back([=]() { EMIT(Inc, bytecode_writer_, dynamic_cast<Variable*>(target_symbol)); });
		}
		if (tn->get_builder_name() == "entity_tail_op_decrement")
		{
			side_effect_stack.top().push_back([=]() { EMIT(Decre, bytecode_writer_, dynamic_cast<Variable*>(target_symbol)); });
		}
	}

	void IRGenerator::statement_block_reader(TreeNode * tn)
	{
		if (tn->get_builder_name() == "statement_block_statement_statement_block_tail")
		{
			statement_reader(tn->get_non_terminal(0));
			TreeNode * tail = tn->get_non_terminal(1);
			while (tail->get_builder_name() == "statement_block_tail_statement_block")
			{
				TreeNode * statement_block = tail->get_non_terminal(0);
				statement_reader(statement_block->get_non_terminal(0));
				tail = statement_block->get_non_terminal(1);
			}
		}
	}

	Symbol * IRGenerator::expr_reader(TreeNode * tn)
	{
		OperandType::Label branch_false(L"branch_false"), branch_end(L"branch_end");
		Symbol * expr_1 = expr_1_reader(tn->get_non_terminal(0), true, nullptr, "");
		TreeNode * expr_tail = tn->get_non_terminal(1);
		if (tn->get_non_terminal(1)->get_builder_name() == "expr_tail_empty")
		{
			return expr_1;
		}
		load_if_not_nullptr(expr_1);

		EMIT(JumpOnFalse, bytecode_writer_, &branch_false);

		Symbol * expr_left = expr_reader(expr_tail->get_non_terminal(1));
		load_if_not_nullptr(expr_left);
		EMIT(Jump, bytecode_writer_, &branch_end);
		bytecode_writer_->bind(branch_false);
		Symbol * expr_right = expr_reader(expr_tail->get_non_terminal(3));
		load_if_not_nullptr(expr_right);
		bytecode_writer_->bind(branch_end);

		return Symbol::Acc;
	}

	IRGenerator::~IRGenerator()
	{
		delete temp_table_;
	}
}


