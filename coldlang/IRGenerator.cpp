#include "stdafx.h"
#include "IRGenerator.h"
#include "BytecodeClass.h"

/* IR Generation Convention
*
* 1. Every time <node_name>_reader is called,
*    `Acc` is allocated as resource, which means
*    it can modify `Acc` for the ONLY purpose of  runtime
*    calculation.
*    It returns either `nullptr` or `Variable *`
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



#define EMIT(bytecode_name, writer, ...) \
	{\
		auto bytecode = IR::BytecodeClass::##bytecode_name();\
		bytecode.init(__VA_ARGS__);\
		writer->emit(bytecode);\
	}


namespace IR {
	using namespace OperandType;
	using namespace BytecodeClass;

	IRGenerator::IRGenerator(Tree * tree,
		SymbolTable * symbol_table,
		BytecodeWriter * bytecode_writer)
		: tree_(tree), symbol_table_(symbol_table), bytecode_writer_(bytecode_writer),
		temp_table_(new TempTable(symbol_table))
	{
	}

	Variable * IRGenerator::look_up_name(Token * token)
	{
		return static_cast<Variable*>(
			symbol_table_->get_by_name(static_cast<Word*>(token)->get_word())
			);
	}



	Symbol * IRGenerator::atom_reader(TreeNode * tn)
	{
		if (tn->get_builder_name() == "atom_identifier")
		{
			return look_up_name(tn->get_terminal(0).get());
		}
		assert(false);
		return nullptr;
	}

	Symbol* IRGenerator::entity_reader(TreeNode* tn)
	{
		if (tn->get_builder_name() == "entity_atom_sub_entity")
		{
			// TODO: for the chain
			return atom_reader(tn->get_non_terminal(0));
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
			if (tn->get_non_terminal(1)->get_builder_name() == "term_tail_empty")
			{
				ret = term;
				goto finish_side_effects;
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

	finish_side_effects:

		for (auto code : expr_side_effects)
		{
			bytecode_writer_->emit(*code);
			delete code;
		}
		expr_side_effects.clear();
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
			auto bytecode = new Inc();
			bytecode->init(target_symbol);
			expr_side_effects.push_back(bytecode);
		}
		if (tn->get_builder_name() == "entity_tail_op_decrement")
		{
			auto bytecode = new Decre();
			bytecode->init(target_symbol);
			expr_side_effects.push_back(bytecode);
		}
	}

	IRGenerator::~IRGenerator()
	{
		delete temp_table_;
	}
}


