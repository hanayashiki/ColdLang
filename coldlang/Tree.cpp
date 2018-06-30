#include "stdafx.h"

TreeNode::TreeNode(string & name, int children_count):
	name_(name), children_count_(children_count), 
	children_(new Child[children_count])
{}

void TreeNode::set_child(int index, Token * terminal)
{
	assert(index >= 0 && index < children_count_);
	children_[index].type = TERMINAL;
	children_[index].terminal = terminal;
}

void TreeNode::set_child(int index, TreeNode * non_terminal)
{
	assert(index >= 0 && index < children_count_);
	children_[index].type = NON_TERMINAL;
	children_[index].non_terminal = non_terminal;
}

TreeNode::~TreeNode()
{
	for (auto i = 0; i < children_count_; i++)
	{
		if (children_[i].type == NON_TERMINAL)
		{
			delete children_[i].non_terminal;
		}
	}
}

Tree::Tree(TreeNode * root) : root_(root)
{
}

Tree::~Tree()
{
	delete root_;
}
