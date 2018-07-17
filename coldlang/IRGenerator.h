#pragma once
#include "stdafx.h"
#include "BytecodeWriter.h"

namespace IR {
	using namespace OperandType;
	class IRGenerator
	{
	private:
		Tree * tree_;
		SymbolTable * symbol_table_;
		TempTable * temp_table_;
		BytecodeWriter * bytecode_writer_;
		list<BytecodeClass::BytecodeBase*> expr_side_effects;
	public:
		IRGenerator(Tree * tree, 
			SymbolTable * symbol_table,
			BytecodeWriter * bytecode_writer);
		~IRGenerator();
		Variable * look_up_name(Token * token);
		Symbol * atom_reader(TreeNode * tn);
		Symbol * entity_reader(TreeNode * tn);
		Symbol * factor_reader(TreeNode * tn);
		Symbol * term_reader(TreeNode * tn, bool head, string && left_builder_name);
		Symbol * expr_5_reader(TreeNode * tn, bool head, Symbol * left_symbol, string && left_builder_name);
		void expr_5_tail_reader(TreeNode * tn, Symbol * target_symbol);
		void term_tail_reader(TreeNode * tn);
		void entity_tail_op_reader(TreeNode * tn, Symbol * target_symbol);
	};

}