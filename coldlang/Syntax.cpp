#include "stdafx.h"

Syntax::Syntax(Lexer* lexer): lexer_(lexer)
{
	tree_meta_ = new TreeMeta();
}

Tree * Syntax::parse()
{
	auto tree_builders = tree_meta_->get_tree_builders("entity");
	TreeNode * root = parse(tree_builders);
	return new Tree(root);
}

TreeNode* Syntax::parse(vector<TreeBuilder*> & tree_builders)
{
	// TODO:
	// errors
	const TreeBuilder * builder = select_tree_builder(tree_builders);
	return parse(builder);
}

TreeNode * Syntax::parse(const TreeBuilder * tree_builder)
{
	TreeNode * result_node = new TreeNode(
		tree_builder->get_name(),
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
			TreeNode * node = parse(tree_meta_->get_tree_builders(component.get_name()));
			result_node->set_child(index, node);
		}
		else
		{
			Token * peek_token = lexer_->peek_token(0);
			if (component.is_good_token(peek_token))
			{
				result_node->set_child(index, peek_token);
				lexer_->next_token();
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

TreeBuilder * Syntax::select_tree_builder(vector<TreeBuilder*>& v) const
{
	for (auto builder : v)
	{
		bool mismatched = false;
		int index = 0;
		// std::wcout << "started: " << endl;
		for (auto peek : builder->get_peek_list())
		{
			// TODO:
			// exception
			Token * peek_token = lexer_->peek_token(index);
			//if (peek_token)
			//	std::wcout << "peek: " << peek_token->to_xml();
			//else
			//	std::wcout << "peek: NULL" << endl;

			// this is temporary
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
		}
		if (mismatched == false)
		{
			return builder;
		}
	}
	// TODO:
	// exception
	std::wcout << "Can not select tree builder" << endl;
	assert(false);
	return nullptr;
}

Syntax::~Syntax()
{
	delete tree_meta_;
}

