#include "stdafx.h"
#include "IRGenerator.h"
#include "BytecodeClass.h"

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
		BytecodeWriter * bytecode_writer)
		: symbol_table_(symbol_table), bytecode_writer_(bytecode_writer),
		function_table_(function_table),
		temp_table_(new TempTable(symbol_table_))
	{
	}

	Variable * IRGenerator::new_name(Token * token)
	{
		return nullptr;
	}

	Variable * IRGenerator::look_up_name(Token * token)
	{
		return static_cast<Variable*>(
			symbol_table_->get_by_name(static_cast<Word*>(token)->get_word())
			);
	}

	Symbol * IRGenerator::self_or_store(Symbol * symbol)
	{
		if (symbol == nullptr) {
			symbol = temp_table_->lend();
			EMIT(StoreAcc, bytecode_writer_, symbol);
		}
		return symbol;
	}

	void IRGenerator::load_if_not_nullptr(Symbol * symbol)
	{
		if (symbol != nullptr)
		{
			EMIT(LoadSymbolToAcc, bytecode_writer_, symbol);
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
		assert(false);
		return nullptr;
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
				EMIT(LoadSymbolToAcc, bytecode_writer_, factor);
			}
		}
		else
		{
			if (left_builder_name == "term_tail_star")
			{
				EMIT(Mul, bytecode_writer_, factor);
			}
			else if (left_builder_name == "term_tail_divide")
			{
				EMIT(Div, bytecode_writer_, factor);
			}
			else if (left_builder_name == "term_tail_mod")
			{
				EMIT(Mod, bytecode_writer_, factor);
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
				if (term == nullptr)
				{
					// will be overwrite, so we should store `Acc` somewhere
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
			if (left_symbol == nullptr) {
				left_symbol = temp_table_->lend();
				EMIT(StoreAcc, bytecode_writer_, left_symbol);
			}
			Symbol * term = term_reader(tn->get_non_terminal(0), true, "");
			if (term == nullptr)
			{
				term = temp_table_->lend();
				EMIT(StoreAcc, bytecode_writer_, term);
				temp_table_->revert(static_cast<Variable*>(term));
			}
			EMIT(LoadSymbolToAcc, bytecode_writer_, left_symbol);
			if (left_symbol->is_temp())
			{
				temp_table_->revert(static_cast<Variable*>(left_symbol));
			}
			if (left_builder_name == "expr_5_tail_add")
				EMIT(Add, bytecode_writer_, term);
			if (left_builder_name == "expr_5_tail_minus")
				EMIT(Sub, bytecode_writer_, term);
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
			side_effect_stack.top().push_back([=]() { EMIT(Inc, bytecode_writer_, target_symbol); });
		}
		if (tn->get_builder_name() == "entity_tail_op_decrement")
		{
			auto bytecode = new Decre();
			bytecode->init(target_symbol);
			side_effect_stack.top().push_back([=]() { EMIT(Decre, bytecode_writer_, target_symbol); });
		}
	}

	Symbol * IRGenerator::statement_reader(TreeNode * tn)
	{
		side_effect_stack.push(SideEffectList());
		if (tn->get_builder_name() == "statement_entity_statement_right")
		{
			Symbol * entity = entity_reader(tn->get_non_terminal(0), true); // TODO: for keyed and named properties
			TreeNode * statement_right = tn->get_non_terminal(1);
			Symbol * expr = expr_reader(statement_right->get_non_terminal(1));
			load_if_not_nullptr(expr);
			EMIT(StoreAcc, bytecode_writer_, entity);
		}
		if (tn->get_builder_name() == "statement_keyword_return_expr")
		{
			Symbol * expr = expr_reader(tn->get_non_terminal(1));
			load_if_not_nullptr(expr);
			EMIT(RetAcc, bytecode_writer_);
		}

		SideEffectList expr_side_effects = side_effect_stack.top();
		side_effect_stack.pop();
		for (const auto side_effect : expr_side_effects)
		{
			side_effect();
		}
		expr_side_effects.clear();
		return nullptr;
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
		Label branch_false(L"branch_false"), branch_end(L"branch_end");
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

		return nullptr;
	}

	IRGenerator::~IRGenerator()
	{
		delete temp_table_;
	}
}


