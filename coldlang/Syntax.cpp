#include "stdafx.h"
#include "BadSyntaxException.h"

Syntax::Syntax(Lexer* lexer): lexer_(lexer)
{
	tree_meta_ = new TreeMeta();
	bad_syntax_flag_ = false;
}

Tree * Syntax::parse(const char * root_name)
{
	auto tree_builders = tree_meta_->get_tree_builders(root_name);
	TreeNode * root = parse(tree_builders);
	Tree * tree = new Tree(root);
	if (bad_syntax_flag_)
	{
		wcout << "Syntax exception caught.\n";
		wcout << tree->to_xml();
		assert(false);
	}
	return tree;
}

TreeNode* Syntax::parse(vector<TreeBuilder*> & tree_builders)
{
	// TODO:
	// errors
	try {
		const TreeBuilder * builder = select_tree_builder(tree_builders);
		return parse(builder);
	} catch (BadSyntaxException e) {
		return nullptr;
	}
}

TreeNode * Syntax::parse(const TreeBuilder * tree_builder)
{
	TreeNode * result_node = new TreeNode(
		tree_builder->get_name(),
		tree_builder->get_builder_name(),
		tree_builder->get_component_list().size()
	);
	int index = 0;
	for (auto component : tree_builder->get_component_list())
	{
		if (component.get_unit_type_() == TreeUnitBuilder::u_t_builder)
		{
			// TODO:
			// find a more favorable way later
			TreeNode * node = parse(reinterpret_cast<const TreeBuilder*>(component.get_t_builder()));
			result_node->set_child(index, node);
		}
		else if (component.get_unit_type_() == TreeUnitBuilder::u_name)
		{
			//wcout << component.get_name() << endl;
			TreeNode * node = parse(tree_meta_->get_tree_builders(component.get_name()));
			result_node->set_child(index, node);
		}
		else
		{
			shared_ptr<Token> peek_token = lexer_->peek_token(0);
			wcout << tree_builder->get_name().c_str() << "/" << tree_builder->get_builder_name().c_str()  << ": " << peek_token->get_raw_string() << endl;
			if (component.is_good_token(peek_token.get()))
			{
				result_node->set_child(index, peek_token);
				lexer_->next_token();
				if (peek_token->get_at_line_head() && component.get_isolate())
				{
					assert(lexer_->peek_token(0)->get_at_line_head() == true || dynamic_cast<Delimiter*>(lexer_->peek_token(0).get()) != nullptr);
				}
			}
			else
			{
				assert(false);
			}
		}
		index++;
	}
	return result_node;
}

TreeBuilder * Syntax::select_tree_builder(vector<TreeBuilder*>& v)
{
	for (auto builder : v)
	{
		if (judge_peek_list(builder->get_peek_list()))
		{
			return builder;
		}
	}
	// TODO:
	// exception
	std::wcout << "Can not select tree builder for " << v[0]->get_name().c_str() <<  endl;
	assert(false);
	bad_syntax_flag_ = true;
	throw BadSyntaxException("Bad syntax: we are expecting ...");
	return nullptr;
}

bool Syntax::judge_peek_list(const vector<TreeUnitBuilder>& peek_list)
{
	bool mismatched = false;
	int index = 0;
	// std::wcout << "started: " << endl;
	for (auto peek : peek_list)
	{
		if (peek.get_unit_type_() != TreeUnitBuilder::u_multi_units) {
			Token * peek_token = lexer_->peek_token(index).get();
			if (peek.get_not() == true)
			{
				if (peek_token == nullptr)
				{
					break;
				}
				if (peek.is_good_token(peek_token))
				{
					mismatched = true;
					break;
				}
			}
			else {
				if (peek_token == nullptr || !peek.is_good_token(peek_token))
				{
					// wcout << peek.is_good_token(peek_token);
					// wcout << " " << peek.get_not() << endl;
					mismatched = true;
					break;
				}
			}
			index++;
		} else
		{
			for (auto or_peek : *peek.get_multi_units())
			{
				bool good = judge_peek_list({ or_peek });
				if (good)
				{
					return true;
				}
			}
			return false;
		}
	}
	return !mismatched;
}

Syntax::~Syntax()
{
	delete tree_meta_;
}

