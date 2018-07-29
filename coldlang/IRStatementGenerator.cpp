#include "stdafx.h"
#include "IRGenerator.h"
#include "BytecodeClass.h"
#include "RuntimeObject.h"
#include "native_functions.h"
#include "NativeSymbol.h"
#include "NativeFunction.h"

namespace IR {
	using namespace OperandType;
	using namespace BytecodeClass;

	Symbol * IRGenerator::statement_reader(TreeNode * tn)
	{
		side_effect_stack.push(SideEffectList());
		if (tn->get_builder_name() == "statement_entity_statement_right")
		{
			Symbol * entity = entity_reader(tn->get_non_terminal(0), true); // TODO: for keyed and named properties
			TreeNode * statement_right = tn->get_non_terminal(1);
			if (statement_right->get_builder_name() == "statement_right_assign_expr") {
				Symbol * expr = expr_reader(statement_right->get_non_terminal(1));
				load_if_not_nullptr(expr);
				emit<StoreAcc>(bytecode_writer_, entity);
			}
		}
		else if (tn->get_builder_name() == "statement_keyword_return_value_expr")
		{
			Symbol * expr = expr_reader(tn->get_non_terminal(1));
			load_if_not_nullptr(expr);
			EMIT(RetAcc, bytecode_writer_);
		}
		else if (tn->get_builder_name() == "statement_keyword_return_value_expr")
		{
			Symbol * expr = expr_reader(tn->get_non_terminal(1));
			load_if_not_nullptr(expr);
			EMIT(RetAcc, bytecode_writer_);
		}
		else if (tn->get_builder_name() == "statement_keyword_return_inline")
		{
			emit<LoadToAcc>(bytecode_writer_, literal_table_->get_none_constant());
			EMIT(RetAcc, bytecode_writer_);
		}
		else if (tn->get_builder_name() == "statement_keyword_if")
		{
			if_reader(tn);
		}
		else if (tn->get_builder_name() == "statement_while")
		{
			while_reader(tn);
		}
		else if (tn->get_builder_name() == "statement_keyword_break")
		{
			break_loop();
		}
		else
		{
			assert(false);
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

	Symbol * IRGenerator::if_reader(TreeNode * tn)
	{
		TreeNode * expr = tn->get_non_terminal(1);
		TreeNode * statement_block = tn->get_non_terminal(3);
		TreeNode * optional_elif_list = tn->get_non_terminal(5);
		TreeNode * optional_else = tn->get_non_terminal(6);

		Label if_end(L"if_end");
		Label elif_begin(L"elif_begin");
		Label else_begin(L"else_begin");

		Label * if_expr_false;
		Label * elif_next;

		// if expr {
		Symbol * expr_result = expr_reader(expr);
		load_if_not_nullptr(expr_result);

		if (optional_elif_list->get_builder_name() == "optional_elif_list_empty")
		{
			/* (1)
			 * if ... {
			 *   ...
			 * } else {
			 *   ...
			 * }
			 * (2)
			 * if ... {
			 *   ...
			 * }
			 */
			if (optional_else->get_builder_name() == "optional_else_empty") {
				if_expr_false = &if_end;
			} else
			{
				if_expr_false = &else_begin;
			}
		} else
		{
			if_expr_false = &elif_begin;
		}
		if (optional_else->get_builder_name() == "optional_else_empty")
		{
			elif_next = &if_end;
		} else
		{
			elif_next = &else_begin;
		}

		EMIT(JumpOnFalse, bytecode_writer_, if_expr_false);

		//    statement
		// }

		statement_block_reader(statement_block);

		if (optional_elif_list->get_builder_name() != "optional_elif_list_empty")
		{
			// elif { ... }
			bytecode_writer_->bind(elif_begin);
			elif_reader(optional_elif_list, elif_next);
		}

		if (optional_else->get_builder_name() != "optional_else_empty")
		{
			// else { ... }
			bytecode_writer_->bind(else_begin);
			else_reader(optional_else);
		}

		bytecode_writer_->bind(if_end);

		return nullptr;
	}

	Symbol * IRGenerator::elif_reader(TreeNode * tn, Label * elif_next)
	{
		if (tn->get_builder_name() == "optional_elif_list_elif") {
			TreeNode * expr = tn->get_non_terminal(1);
			TreeNode * statement_block = tn->get_non_terminal(3);
			TreeNode * elif_list = tn->get_non_terminal(5);

			Label elif_false(L"elif_false");
			Symbol * expr_result = expr_reader(expr);
			load_if_not_nullptr(expr_result);

			if (elif_list->get_builder_name() == "optional_elif_list_empty")
			{
				EMIT(JumpOnFalse, bytecode_writer_, elif_next);
			}
			else
			{
				EMIT(JumpOnFalse, bytecode_writer_, &elif_false);
			}

			statement_block_reader(statement_block);

			if (elif_list->get_builder_name() != "optional_elif_list_empty")
			{
				bytecode_writer_->bind(elif_false);
				elif_reader(elif_list, elif_next);
			}
		}
		return nullptr;
	}

	Symbol * IRGenerator::else_reader(TreeNode * tn)
	{
		TreeNode * statement_block = tn->get_non_terminal(2);
		statement_block_reader(statement_block);
		return nullptr;
	}

	Symbol * IRGenerator::while_reader(TreeNode * tn)
	{
		TreeNode * expr = tn->get_non_terminal(1);
		TreeNode * statement_block = tn->get_non_terminal(3);
		Label condition(L"while_condition"), while_end(L"while_end");
		bytecode_writer_->bind(condition);
		Symbol * expr_result = expr_reader(expr);
		load_if_not_nullptr(expr_result);
		EMIT(JumpOnFalse, bytecode_writer_, &while_end);
		loop_end_stack.push(while_end);
		statement_block_reader(statement_block);
		loop_end_stack.pop();
		EMIT(Jump, bytecode_writer_, &condition);
		bytecode_writer_->bind(while_end);
		return nullptr;
	}

	void IRGenerator::break_loop()
	{
		if (loop_end_stack.size() == 0)
		{
			// todo: throw properly
			assert(false);
		} else
		{
			EMIT(Jump, bytecode_writer_, &loop_end_stack.top());
		}
	}
}