#include "stdafx.h"
#include "IRGenerator.h"
#include "BytecodeClass.h"

namespace IR {
	using namespace OperandType;
	using namespace BytecodeClass;

	Symbol * IRGenerator::expr_4_reader(TreeNode * tn, bool head, Symbol * left_symbol, string && left_builder_name)
	{
		Symbol * ret = nullptr;
		Symbol * new_left_symbol = nullptr;
		if (head)
		{
			Symbol * expr_5 = expr_5_reader(tn->get_non_terminal(0), true, nullptr, "");
			if (tn->get_non_terminal(1)->get_builder_name() == "expr_4_tail_empty")
			{
				ret = expr_5;
				return ret;
			}
			else
			{
				if (expr_5 == nullptr)
				{
					// will be overwrite, so we should store `Acc` somewhere
					Variable * temp = temp_table_->lend();
					new_left_symbol = temp;
					EMIT(StoreAcc, bytecode_writer_, temp);
				}
				else
				{
					new_left_symbol = expr_5;
				}
			}
		}
		else
		{
			left_symbol = self_or_store(left_symbol);
			Symbol * expr_5 = expr_5_reader(tn->get_non_terminal(0), true, nullptr, "");
			if (expr_5 == nullptr)
			{
				expr_5 = temp_table_->lend();
				EMIT(StoreAcc, bytecode_writer_, expr_5);
				temp_table_->revert(static_cast<Variable*>(expr_5));
			}
			load_variable_or_literal(left_symbol);
			if (left_symbol->is_temp())
			{
				temp_table_->revert(static_cast<Variable*>(left_symbol));
			}
			if (left_builder_name == "greater_equal_expr_4")
				EMIT(GreaterEqual, bytecode_writer_, expr_5);
			if (left_builder_name == "greater_expr_4")
				EMIT(GreaterThan, bytecode_writer_, expr_5);
			if (left_builder_name == "less_equal_expr_4")
				EMIT(LessEqual, bytecode_writer_, expr_5);
			if (left_builder_name == "less_expr_4")
				EMIT(LessThan, bytecode_writer_, expr_5);

		}
		expr_4_tail_reader(tn->get_non_terminal(1), new_left_symbol);
		return ret;
	}

	void IRGenerator::expr_4_tail_reader(TreeNode * tn, Symbol * target_symbol)
	{
		if (tn->get_builder_name() != "expr_4_tail_empty") {
			expr_4_reader(tn->get_non_terminal(1), false, target_symbol, tn->get_builder_name());
		}
	}
	Symbol * IRGenerator::expr_3_reader(TreeNode * tn, bool head, Symbol * left_symbol, string && left_builder_name)
	{
		Symbol * ret = nullptr;
		Symbol * new_left_symbol = nullptr;
		if (head)
		{
			Symbol * expr_4 = expr_4_reader(tn->get_non_terminal(0), true, nullptr, "");
			if (tn->get_non_terminal(1)->get_builder_name() == "expr_3_tail_empty")
			{
				ret = expr_4;
				return ret;
			}
			else
			{
				if (expr_4 == nullptr)
				{
					// will be overwrite, so we should store `Acc` somewhere
					Variable * temp = temp_table_->lend();
					new_left_symbol = temp;
					EMIT(StoreAcc, bytecode_writer_, temp);
				}
				else
				{
					new_left_symbol = expr_4;
				}
			}
		}
		else
		{
			left_symbol = self_or_store(left_symbol);
			Symbol * expr_4 = expr_4_reader(tn->get_non_terminal(0), true, nullptr, "");
			if (expr_4 == nullptr)
			{
				expr_4 = temp_table_->lend();
				EMIT(StoreAcc, bytecode_writer_, expr_4);
				temp_table_->revert(static_cast<Variable*>(expr_4));
			}
			load_variable_or_literal(left_symbol);
			if (left_symbol->is_temp())
			{
				temp_table_->revert(static_cast<Variable*>(left_symbol));
			}
			if (left_builder_name == "not_equal_expr_3")
				EMIT(NotEqual, bytecode_writer_, expr_4);
			if (left_builder_name == "equal_expr_3")
				EMIT(Equal, bytecode_writer_, expr_4);

		}
		expr_3_tail_reader(tn->get_non_terminal(1), new_left_symbol);
		return ret;
	}

	void IRGenerator::expr_3_tail_reader(TreeNode * tn, Symbol * target_symbol)
	{
		if (tn->get_builder_name() != "expr_3_tail_empty") {
			expr_3_reader(tn->get_non_terminal(1), false, target_symbol, tn->get_builder_name());
		}
	}
	Symbol * IRGenerator::expr_2_reader(TreeNode * tn, bool head, Symbol * left_symbol, string && left_builder_name)
	{
		Symbol * ret = nullptr;
		Symbol * new_left_symbol = nullptr;
		if (head)
		{
			Symbol * expr_3 = expr_3_reader(tn->get_non_terminal(0), true, nullptr, "");
			if (tn->get_non_terminal(1)->get_builder_name() == "expr_2_tail_empty")
			{
				ret = expr_3;
				return ret;
			}
			else
			{
				if (expr_3 == nullptr)
				{
					// will be overwrite, so we should store `Acc` somewhere
					Variable * temp = temp_table_->lend();
					new_left_symbol = temp;
					EMIT(StoreAcc, bytecode_writer_, temp);
				}
				else
				{
					new_left_symbol = expr_3;
				}
			}
		}
		else
		{
			left_symbol = self_or_store(left_symbol);
			Symbol * expr_3 = expr_3_reader(tn->get_non_terminal(0), true, nullptr, "");
			if (expr_3 == nullptr)
			{
				expr_3 = temp_table_->lend();
				EMIT(StoreAcc, bytecode_writer_, expr_3);
				temp_table_->revert(static_cast<Variable*>(expr_3));
			}
			load_variable_or_literal(left_symbol);
			if (left_symbol->is_temp())
			{
				temp_table_->revert(static_cast<Variable*>(left_symbol));
			}
			if (left_builder_name == "expr_2_tail_and")
				EMIT(And, bytecode_writer_, expr_3);

		}
		expr_2_tail_reader(tn->get_non_terminal(1), new_left_symbol);
		return ret;
	}

	void IRGenerator::expr_2_tail_reader(TreeNode * tn, Symbol * target_symbol)
	{
		if (tn->get_builder_name() != "expr_2_tail_empty") {
			expr_2_reader(tn->get_non_terminal(1), false, target_symbol, tn->get_builder_name());
		}
	}
	Symbol * IRGenerator::expr_1_reader(TreeNode * tn, bool head, Symbol * left_symbol, string && left_builder_name)
	{
		Symbol * ret = nullptr;
		Symbol * new_left_symbol = nullptr;
		if (head)
		{
			Symbol * expr_2 = expr_2_reader(tn->get_non_terminal(0), true, nullptr, "");
			if (tn->get_non_terminal(1)->get_builder_name() == "expr_1_tail_empty")
			{
				ret = expr_2;
				return ret;
			}
			else
			{
				if (expr_2 == nullptr)
				{
					// will be overwrite, so we should store `Acc` somewhere
					Variable * temp = temp_table_->lend();
					new_left_symbol = temp;
					EMIT(StoreAcc, bytecode_writer_, temp);
				}
				else
				{
					new_left_symbol = expr_2;
				}
			}
		}
		else
		{
			left_symbol = self_or_store(left_symbol);
			Symbol * expr_2 = expr_2_reader(tn->get_non_terminal(0), true, nullptr, "");
			if (expr_2 == nullptr)
			{
				expr_2 = temp_table_->lend();
				EMIT(StoreAcc, bytecode_writer_, expr_2);
				temp_table_->revert(static_cast<Variable*>(expr_2));
			}
			load_variable_or_literal(left_symbol);
			if (left_symbol->is_temp())
			{
				temp_table_->revert(static_cast<Variable*>(left_symbol));
			}
			if (left_builder_name == "expr_1_tail_or")
				EMIT(Or, bytecode_writer_, expr_2);

		}
		expr_1_tail_reader(tn->get_non_terminal(1), new_left_symbol);
		return ret;
	}

	void IRGenerator::expr_1_tail_reader(TreeNode * tn, Symbol * target_symbol)
	{
		if (tn->get_builder_name() != "expr_1_tail_empty") {
			expr_1_reader(tn->get_non_terminal(1), false, target_symbol, tn->get_builder_name());
		}
	}

}