#pragma once
#include "stdafx.h"

class TreeNode;

class Tree
{
private:
	TreeNode * root_;
public:
	// @owns: TreeNode* root
	Tree(TreeNode * root);
	~Tree();
	wstring to_xml(int max_tag_char = 4);
	TreeNode * get_root();
};

class TreeNode
{
private:
	enum ChildType
	{
		Terminal,
		NonTerminal
	};
	struct Child
	{
		ChildType type;
		union
		{
			shared_ptr<Token> terminal;
			TreeNode* non_terminal;
		};
		Child()
			: terminal(shared_ptr<Token>()), type(Terminal)
		{
		}
		~Child()
		{	
		}
	};
	string name_;
	string builder_name;
	int children_count_;
	Child * children_;
public:
	TreeNode(string name, string builder_name, int children_count);
	void set_child(int index, shared_ptr<Token> & terminal);
	void set_child(int index, TreeNode* non_terminal);
	shared_ptr<Token> & get_terminal(int index);
	TreeNode * get_non_terminal(int index);
	string get_builder_name();
	int get_children_count_();
	wstring to_xml(int indent = 0, int indent_size = 2, int max_char = 4) const;
	~TreeNode();
};