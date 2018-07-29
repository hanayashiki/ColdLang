#pragma once
#include "stdafx.h"
#include "BytecodeWriter.h"
#include <functional>
#include <stack>

#define EMIT(bytecode_name, writer, ...) \
	{\
		IR::BytecodeClass::##bytecode_name bytecode(__VA_ARGS__);\
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
		LiteralTable * literal_table_;
		ConstantTable * constant_table_;
		BytecodeWriter * bytecode_writer_;
		typedef list<function<void()>> SideEffectList;
		stack<SideEffectList> side_effect_stack;

		void initialize_native_symbols();
		Variable * new_name(Token * token);
		Symbol * look_up_name(Token * token);
		Symbol * self_or_store(Symbol * symbol);
		void load_if_not_nullptr(Symbol * symbol);
		void load_variable_or_literal(Symbol * symbol);
		template<typename RealTokenType>
		Literal * add_literal(shared_ptr<Token> & token, Runtime::RuntimeObject * rto);
	public:
		IRGenerator(SymbolTable * symbol_table, FunctionTable * function_table, LiteralTable * literal_table, ConstantTable * constant_table, BytecodeWriter * bytecode_writer);
		~IRGenerator();

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

		vector<Variable*> comma_identifiers_reader(TreeNode * tn);
		Symbol * func_def_and_optional_call_reader(TreeNode * tn);

		vector<Symbol*> comma_exprs_reader(TreeNode * tn);
	};

}