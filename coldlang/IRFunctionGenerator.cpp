#include "stdafx.h"
#include "IRGenerator.h"
#include "BytecodeClass.h"
#include "AssignTarget.h"
#include "Literal.h"


namespace IR {
	using namespace OperandType;
	using namespace BytecodeClass;

	vector<Variable*> IRGenerator::comma_identifiers_reader(TreeNode * tn)
	{
		vector<Variable*> comma_list;
		// recursively travel TreeNode "comma_identifiers"

		if (tn->get_builder_name() == "comma_identifiers_identifier_comma_identifiers_tail") {
			for (auto word = tn->get_terminal(0);;
				tn = tn->get_non_terminal(1)->get_non_terminal(1)
				)
			{
				Variable * variable = new Variable(tn->get_terminal(0));
				if (symbol_table_->define(variable))
				{
					comma_list.push_back(variable);
				}
				else
				{
					delete variable;
					throw SymbolException("Redefined parameter: ", word);
				}
				if (tn->get_non_terminal(1)->get_builder_name() !=
					"comma_identifier_tail_comma_comma_identifiers")
				{
					break;
				}
			}
			return std::move(comma_list);
		} 
		if (tn->get_builder_name() == "comma_identifiers_empty")
		{
			return std::move(comma_list);
		}
		assert(false);
	}

	Symbol * IRGenerator::func_def_and_optional_call_reader(TreeNode * tn)
	{
		shared_ptr<Token> keyword_token = tn->get_terminal(0);
		TreeNode * comma_identifiers = tn->get_non_terminal(1);
		TreeNode * func_body = tn->get_non_terminal(2);
		TreeNode * optional_call = tn->get_non_terminal(3);

		TreeNode * statement_block = func_body->get_non_terminal(1);

		vector<Variable*> param_list = comma_identifiers_reader(comma_identifiers);
		Function * function = new Function(keyword_token, param_list);
		function_table_->add(function);

		Literal * func_literal = new Literal(keyword_token, function);
		// TODO: deal with Literal

		side_effect_stack.top().push_back([=]()
		{
			wcout << endl;
			wcout << "code for " << func_literal->to_string() << endl << endl;
			wcout << "-----------------------------------------------------------" << endl << endl;
			statement_block_reader(statement_block);
			wcout << endl;
			wcout << "-----------------------------------------------------------" << endl << endl;
		});
		// TODO: optional_caller
		return func_literal;
	}

}
