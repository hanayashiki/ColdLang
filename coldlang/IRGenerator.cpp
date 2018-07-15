#include "stdafx.h"
#include "IRGenerator.h"
#include "BytecodeClass.h"

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
		: tree_(tree), symbol_table_(symbol_table), bytecode_writer_(bytecode_writer)
	{
	}
	/* Convention:
	 *
	 * Accumulator is good to overwrite each time we visit a tree_node
	 *
	 * For each binary operation, the left value is passed 
	 * through Accumulator as parameter, 
	 * and the result will be store in Accumulator
	 *
	 */

	Symbol * IRGenerator::factor_reader(TreeNode * tree_node)
	{
		Symbol * entity = entity_reader(tree_node->get_non_terminal(0));
		// entity entity_tail_op
		entity_tail_op_reader(tree_node->get_non_terminal(1), entity);
		return entity;
	}

	void IRGenerator::entity_tail_op_reader(TreeNode * tree_node, Symbol* entity)
	{
		// Delimiter::increment
		if (tree_node->get_builder_name() == "entity_tail_op_increment")
		{
			if (entity->get_value_type() != Symbol::LValue)
			{
				assert(false);
			}
			EMIT(Inc, bytecode_writer_, entity);
			return;
		}
		if (tree_node->get_builder_name() == "entity_tail_op_decrement")
		{
			if (entity->get_value_type() != Symbol::LValue)
			{
				assert(false);
			}
			EMIT(Decre, bytecode_writer_, entity);
			return;
		}
	}

	Symbol * IRGenerator::entity_reader(TreeNode * tree_node)
	{
		if (tree_node->get_builder_name() == "entity_atom_sub_entity")
		{
			// atom [empty]sub_entity
			if (tree_node->get_non_terminal(1)->get_children_count_() == 0)
			{
				return atom_reader(tree_node->get_non_terminal(0));
			}
			// atom [non-empty]sub_entity
			// todo: implement hidden class

			assert(false);
		}

		assert(false);
	}

	Symbol * IRGenerator::atom_reader(TreeNode * tree_node)
	{
		if (tree_node->get_builder_name() == "atom_identifier")
		{
			auto p_token = static_cast<Word*>(tree_node->get_terminal(0).get());
			auto token_name = p_token->get_word();
			Symbol * symbol = symbol_table_->get_by_name(token_name);
			EMIT(LoadSymbolToAcc, bytecode_writer_, symbol);
			return symbol;
		}

		assert(false);
		return nullptr;
	}
}
