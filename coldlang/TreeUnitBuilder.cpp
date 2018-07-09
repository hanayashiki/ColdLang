#include "stdafx.h"

TreeUnitBuilder::TreeUnitBuilder(): 
	unit_type_(u_unknown),
	optional_(false),
	not_(false)
{	

}

TreeUnitBuilder::TreeUnitBuilder(const char* n) :
	unit_type_(u_name),
	optional_(false),
	not_(false)
{
	if (strcmp(n, "integer") == 0)
	{
		unit_type_ = u_integer;
	}
	else if (strcmp(n, "float") == 0)
	{
		unit_type_ = u_float;
	}
	value_.name = n;
}

TreeUnitBuilder::TreeUnitBuilder(Word::WordType word_type) :
	unit_type_(u_word),
	optional_(false),
	not_(false)
{
	value_.word_type = word_type;
}

TreeUnitBuilder::TreeUnitBuilder(Delimiter::DelimiterType delimiter_type) :
	unit_type_(u_delimiter),
	optional_(false),
	not_(false)
{
	value_.delimiter_type = delimiter_type;
}

TreeUnitBuilder::TreeUnitBuilder(String::StringType string_type) :
	unit_type_(u_string),
	optional_(false),
	not_(false)
{
	value_.string_type = string_type;
}

TreeUnitBuilder::TreeUnitBuilder(TreeBuilderNode* tb) :
	unit_type_(u_t_builder),
	optional_(false),
	not_(false)
{
	value_.t_builder = tb;
}

TreeUnitBuilder::TreeUnitBuilder(initializer_list<TreeUnitBuilder> & or_list) :
	unit_type_(u_multi_units),
	optional_(false),
	not_(false)
{
	value_.multi_units = new vector<TreeUnitBuilder>(or_list);
}

TreeUnitBuilder::~TreeUnitBuilder()
{
	if (get_unit_type_() == u_multi_units)
	{
		// wcout << "u_multi_units is destoryed\n" << endl;
		delete value_.multi_units;
	}
}

TreeUnitBuilder::TreeUnitBuilder(const TreeUnitBuilder & tb)
{
	unit_type_ = tb.unit_type_;
	value_ = tb.value_;
	optional_ = tb.optional_;
	not_ = tb.not_;
	if (tb.get_unit_type_() == u_multi_units)
	{
		value_.multi_units = new vector<TreeUnitBuilder>(*tb.value_.multi_units);
	}
}

void TreeUnitBuilder::set_optional(const bool & optional)
{
	optional_ = optional;
}

bool TreeUnitBuilder::get_optional() const
{
	return optional_;
}

void TreeUnitBuilder::set_not(const bool & _not)
{
	not_ = _not;
}

bool TreeUnitBuilder::get_not() const
{
	return not_;
}

const char * TreeUnitBuilder::get_name() const
{
	assert(unit_type_ == u_name);
	return value_.name;
}

Word::WordType TreeUnitBuilder::get_word_type() const
{
	assert(unit_type_ == u_word);
	return value_.word_type;
}

Delimiter::DelimiterType TreeUnitBuilder::get_delimiter_type() const
{
	assert(unit_type_ == u_delimiter);
	return value_.delimiter_type;
}

String::StringType TreeUnitBuilder::get_string_type() const
{
	assert(unit_type_ == u_string);
	return value_.string_type;
}

const TreeBuilderNode * TreeUnitBuilder::get_t_builder() const
{
	assert(unit_type_ == u_t_builder);
	return value_.t_builder;
}

const vector<TreeUnitBuilder>* TreeUnitBuilder::get_multi_units() const
{
	assert(unit_type_ == u_multi_units);
	return value_.multi_units;
}

TreeUnitBuilder::UnitType TreeUnitBuilder::get_unit_type_() const
{
	return unit_type_;
}

bool TreeUnitBuilder::is_good_token(Token* token) const
{
	assert(get_unit_type_() != u_t_builder);
	if (typeid(*token) == typeid(Word))
	{
		if (get_unit_type_() == u_word) 
		{
			return get_word_type() == reinterpret_cast<Word*>(token)->get_type();
		}
	}
	if (typeid(*token) == typeid(Delimiter))
	{
		if (get_unit_type_() == u_delimiter)
		{
			return get_delimiter_type() == reinterpret_cast<Word*>(token)->get_type();
		}
	}
	if (typeid(*token) == typeid(String))
	{
		if (get_unit_type_() == u_string)
		{
			return get_string_type() == reinterpret_cast<String*>(token)->get_type();
		}
	}
	if (typeid(*token) == typeid(Integer))
	{
		return get_unit_type_() == u_integer;
	}
	if (typeid(*token) == typeid(Float))
	{
		return get_unit_type_() == u_float;
	}
	return false;
}








