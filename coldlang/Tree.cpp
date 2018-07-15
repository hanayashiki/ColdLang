#include "stdafx.h"

TreeNode::TreeNode(string name, string builder_name, int children_count):
	name_(name), builder_name(builder_name), children_count_(children_count), 
	children_(new Child[children_count])
{}

void TreeNode::set_child(int index, shared_ptr<Token> & terminal)
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

shared_ptr<Token> & TreeNode::get_terminal(int index)
{
	assert(index >= 0 && index < children_count_);
	assert(children_[index].type == Terminal);
	return children_[index].terminal;
}


TreeNode * TreeNode::get_non_terminal(int index)
{
	assert(index >= 0 && index < children_count_);
	assert(children_[index].type == NonTerminal);
	return children_[index].non_terminal;
}

string TreeNode::get_builder_name()
{
	return builder_name;
}

int TreeNode::get_children_count_()
{
	return children_count_;
}

wstring TreeNode::to_xml(int indent, int indent_size, int max_char) const
{
	wstring serialized;
	serialized.reserve(128);
	serialized += tutils::get_indent(indent, indent_size);
	serialized += tutils::to_xml_begin_tag(tutils::ascii_string_to_wstring(this->name_), max_char);
	
	for (int i = 0; i < children_count_; i++)
	{
		if (children_[i].type == Terminal)
		{
			serialized += tutils::get_indent(indent + 1, indent_size);
			if (children_[i].terminal != nullptr) {
				serialized += children_[i].terminal->to_xml();
			} else
			{
				serialized += L"<UNKNOWN/>\n";
			}
		} else
		{
			if (children_[i].non_terminal != nullptr) {
				serialized += children_[i].non_terminal->to_xml(indent + 1, indent_size, max_char);
			} else
			{
				serialized += tutils::get_indent(indent + 1, indent_size);
				serialized += L"<UNKNOWN/>\n";
			}
		}
	}
	serialized += tutils::get_indent(indent, indent_size);
	serialized += tutils::to_xml_end_tag(tutils::ascii_string_to_wstring(this->name_), max_char);
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
		if (children_[i].type == Terminal)
		{
			// wcout << "~TreeNode() token use count :" << children_[i].terminal.use_count() << endl;
			children_[i].terminal.~shared_ptr();
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

wstring Tree::to_xml(int max_tag_char)
{
	return root_->to_xml(0, 3, max_tag_char);
}

TreeNode * Tree::get_root()
{
	return root_;
}