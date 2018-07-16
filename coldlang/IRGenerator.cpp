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
	 * Accumulator is good to overwrite each time we visit a tree_node, unless acc_overwrite is false
	 *
	 * For each binary operation, the left value is passed 
	 * through Accumulator as parameter, 
	 * and the result will be store in Accumulator
	 *
	 *
	 */

	Symbol * IRGenerator::factor_reader(TreeNode * tree_node, bool acc_overwrite)
	{
		Symbol * entity = entity_reader(tree_node->get_non_terminal(0), acc_overwrite);
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
			auto inc = new Inc();
			inc->init(entity);
			term_side_effects.push_back(inc);
			//EMIT(Inc, bytecode_writer_, entity);
			return;
		}
		if (tree_node->get_builder_name() == "entity_tail_op_decrement")
		{
			if (entity->get_value_type() != Symbol::LValue)
			{
				assert(false);
			}
			auto decre = new Decre();
			decre->init(entity);
			term_side_effects.push_back(decre);
			//EMIT(Decre, bytecode_writer_, entity);
			return;
		}
	}

	Symbol * IRGenerator::entity_reader(TreeNode * tree_node, bool acc_overwrite)
	{
		if (tree_node->get_builder_name() == "entity_atom_sub_entity")
		{
			// atom [empty]sub_entity
			if (tree_node->get_non_terminal(1)->get_children_count_() == 0)
			{
				return atom_reader(tree_node->get_non_terminal(0), acc_overwrite);
			}
			// atom [non-empty]sub_entity
			// todo: implement hidden class

			assert(false);
		}

		assert(false);
	}

	Symbol * IRGenerator::atom_reader(TreeNode * tree_node, bool acc_overwrite)
	{
		if (tree_node->get_builder_name() == "atom_identifier")
		{
			auto p_token = static_cast<Word*>(tree_node->get_terminal(0).get());
			auto token_name = p_token->get_word();
			Symbol * symbol = symbol_table_->get_by_name(token_name);
			// wcout << "atom reader got: " << token_name << endl;
			// wcout << "acc_overwrite: " << acc_overwrite << endl;
			if (acc_overwrite) {
				EMIT(LoadSymbolToAcc, bytecode_writer_, symbol);
			}
			return symbol;
		}

		assert(false);
		return nullptr;
	}

	OperandType::Symbol* IRGenerator::term_reader(TreeNode* tree_node, Symbol * left_symbol, string && left_builder)
	{
		Symbol * new_left_symbol = nullptr;

		// wcout << "left builder: " << left_builder.c_str() << endl;
		if (left_builder == "term_tail_star") {
			auto right_symbol = factor_reader(tree_node->get_non_terminal(0), false);
			EMIT(Mul, bytecode_writer_, right_symbol);
		}
		else if (left_builder == "term_tail_divide")
		{
			auto right_symbol = factor_reader(tree_node->get_non_terminal(0), false);
			EMIT(Div, bytecode_writer_, right_symbol);
		}
		else if (left_builder == "term_tail_mod")
		{
			auto right_symbol = factor_reader(tree_node->get_non_terminal(0), false);
			EMIT(Mod, bytecode_writer_, right_symbol);
		} else
		{
			factor_reader(tree_node->get_non_terminal(0), true);
		}

		term_tail_reader(tree_node->get_non_terminal(1), new_left_symbol);

		for (auto bytecode : term_side_effects)
		{
			bytecode_writer_->emit(*bytecode);
			delete bytecode;
		}
		term_side_effects.clear();

		return nullptr;
	}


	OperandType::Symbol * IRGenerator::term_tail_reader(TreeNode * tree_node, Symbol * left_symbol)
	{
		if (tree_node->get_builder_name() != "term_tail_empty")
		{
			term_reader(tree_node->get_non_terminal(1), left_symbol, tree_node->get_builder_name());
		}
		return nullptr;
	}
}
