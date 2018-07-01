#include "stdafx.h"

TreeNode::TreeNode(string name, int children_count):
	name_(name), children_count_(children_count), 
	children_(new Child[children_count])
{}

void TreeNode::set_child(int index, Token * terminal)
{
	assert(index >= 0 && index < children_count_);
	children_[index].type = Terminal;
	children_[index].terminal = terminal;
}

void TreeNode::set_child(int index, TreeNode * non_terminal)
{
	assert(index >= 0 && index < children_count_);
	children_[index].type = NonTerminal;
	children_[index].non_terminal = non_terminal;
}

wstring TreeNode::to_xml(int indent, int indent_size) const
{
	wstring serialized;
	serialized.reserve(128);
	serialized += tutils::get_indent(indent, indent_size);
	serialized += tutils::to_xml_begin_tag(tutils::ascii_string_to_wstring(this->name_));
	
	for (int i = 0; i < children_count_; i++)
	{
		if (children_[i].type == Terminal)
		{
			serialized += tutils::get_indent(indent + 1, indent_size);
			serialized += children_[i].terminal->to_xml();
		} else
		{
			serialized += children_[i].non_terminal->to_xml(indent + 1, indent_size);
		}
	}
	serialized += tutils::get_indent(indent, indent_size);
	serialized += tutils::to_xml_end_tag(tutils::ascii_string_to_wstring(this->name_));
	return serialized;
}

TreeNode::~TreeNode()
{
	for (auto i = 0; i < children_count_; i++)
	{
		if (children_[i].type == NonTerminal)
		{
			delete children_[i].non_terminal;
		}
	}
	delete[] children_;
}

Tree::Tree(TreeNode * root) : root_(root)
{
}

Tree::~Tree()
{
	delete root_;
}

wstring Tree::to_xml()
{
	return root_->to_xml();
}
