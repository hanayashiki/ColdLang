#pragma once
#include "stdafx.h"
#include "BytecodeWriter.h"

namespace IR {

	class IRGenerator
	{
	private:
		Tree * tree_;
		SymbolTable * symbol_table_;
		BytecodeWriter * bytecode_writer_;
	public:
		IRGenerator(Tree * tree, 
			SymbolTable * symbol_table, 
			BytecodeWriter * bytecode_writer);
		OperandType::Symbol * factor_reader(TreeNode * tree_node);
		void entity_tail_op_reader(TreeNode * tree_node, OperandType::Symbol * entity);
		OperandType::Symbol * entity_reader(TreeNode * tree_node);
		OperandType::Symbol * atom_reader(TreeNode * tree_node);
	};
}