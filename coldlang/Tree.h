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
			Token* terminal;
			TreeNode* non_terminal;
		};
	};
	string name_;
	int children_count_;
	Child * children_;
public:
	TreeNode(string name, int children_count);
	void set_child(int index, Token* terminal);
	void set_child(int index, TreeNode* non_terminal);
	wstring to_xml(int indent = 0, int indent_size = 2, int max_char = 4) const;
	~TreeNode();
};