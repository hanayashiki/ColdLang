#include "stdafx.h"

TreeBuilder::TreeBuilder(
	const string && name,
	const std::initializer_list<TreeUnitBuilder> peek_list,
	const std::initializer_list<TreeUnitBuilder> component_list,
	const string && builder_name
):
	name_(name),
	peek_list_(peek_list),
	component_list_(component_list),
	builder_name_(builder_name)
{
	
}

