#include "stdafx.h"

TreeBuilder::TreeBuilder(
	const string name,
	const std::initializer_list<TreeUnitBuilder> peek_list,
	const std::initializer_list<TreeUnitBuilder> component_list
):
	name_(name),
	peek_list_(peek_list),
	component_list_(component_list)
{
	
}

