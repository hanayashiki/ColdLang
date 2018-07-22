#pragma once
#include "stdafx.h"
#include "BytecodeWriter.h"
#include <functional>
#include <stack>

#define EMIT(bytecode_name, writer, ...) \
	{\
		auto bytecode = IR::BytecodeClass::##bytecode_name();\
		bytecode.init(__VA_ARGS__);\
		writer->emit(bytecode);\
	}

namespace IR {
	class AssignTarget;
	using namespace OperandType;
	class IRGenerator
	{
	private:
		SymbolTable * symbol_table_;
		TempTable * temp_table_;
		FunctionTable * function_table_;
		BytecodeWriter * bytecode_writer_;
		typedef list<function<void()>> SideEffectList;
		stack<SideEffectList> side_effect_stack;
	public:
		IRGenerator(SymbolTable * symbol_table, FunctionTable * function_table, BytecodeWriter * bytecode_writer);
		Variable * new_name(Token * token);
		~IRGenerator();
		Variable * look_up_name(Token * token);
		Symbol * self_or_store(Symbol * symbol);
		void load_if_not_nullptr(Symbol * symbol);
		Symbol * atom_reader(TreeNode * tn, bool left);
		Symbol * entity_reader(TreeNode * tn, bool create = false);
		Symbol * factor_reader(TreeNode * tn);
		Symbol * term_reader(TreeNode * tn, bool head, string && left_builder_name);
		Symbol * expr_5_reader(TreeNode * tn, bool head, Symbol * left_symbol, string && left_builder_name);
		void expr_5_tail_reader(TreeNode * tn, Symbol * target_symbol);

		Symbol * expr_4_reader(TreeNode * tn, bool head, Symbol * left_symbol, string && left_builder_name);
		void expr_4_tail_reader(TreeNode * tn, Symbol * target_symbol);
		Symbol * expr_3_reader(TreeNode * tn, bool head, Symbol * left_symbol, string && left_builder_name);
		void expr_3_tail_reader(TreeNode * tn, Symbol * target_symbol);
		Symbol * expr_2_reader(TreeNode * tn, bool head, Symbol * left_symbol, string && left_builder_name);
		void expr_2_tail_reader(TreeNode * tn, Symbol * target_symbol);
		Symbol * expr_1_reader(TreeNode * tn, bool head, Symbol * left_symbol, string && left_builder_name);
		void expr_1_tail_reader(TreeNode * tn, Symbol * target_symbol);

		Symbol * expr_reader(TreeNode * tn);

		void term_tail_reader(TreeNode * tn);
		void entity_tail_op_reader(TreeNode * tn, Symbol * target_symbol);

		Symbol * statement_reader(TreeNode * tn);
		void statement_block_reader(TreeNode * tn);

		vector<Variable *> comma_identifiers_reader(TreeNode * tn);
		Symbol * func_def_and_optional_call_reader(TreeNode * tn);

	};

}