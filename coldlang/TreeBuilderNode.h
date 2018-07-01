#pragma once
#include "stdafx.h"

class TreeUnitBuilder;

class TreeBuilderNode {

};

class TreeBuilder : public TreeBuilderNode {
private:
	string name_;
	std::vector<TreeUnitBuilder> peek_list_;
	std::vector<TreeUnitBuilder> component_list_;
public:
	TreeBuilder(
		string name,
		const std::initializer_list<TreeUnitBuilder> peek_list,
		const std::initializer_list<TreeUnitBuilder> component_list
	);
	string get_name() const
	{
		return name_;
	}
	const std::vector<TreeUnitBuilder> & get_peek_list() const 
	{
		return peek_list_;
	}
	const std::vector<TreeUnitBuilder> & get_component_list() const
	{
		return component_list_;
	}
};

class TreeUnitBuilder : public TreeBuilderNode {
public:
	enum UnitType {
		u_unknown,
		u_name,
		u_word,
		u_delimiter,
		u_float,
		u_integer,
		u_string,
		u_token,
		u_t_builder
	};
private:
	union Value
	{
		const char* name;
		Word::WordType word_type;
		Delimiter::DelimiterType delimiter_type;
		String::StringType string_type;
		TreeBuilderNode* t_builder;
	};
	UnitType unit_type_;
	Value value_;
	bool optional_;
	bool not_;
public:
	TreeUnitBuilder();
	TreeUnitBuilder(const char* n);
	TreeUnitBuilder(Word::WordType word_type);
	TreeUnitBuilder(Delimiter::DelimiterType delimiter_type);
	TreeUnitBuilder(String::StringType string_type);
	TreeUnitBuilder(TreeBuilderNode * tb);

	void set_optional(const bool & optional);
	bool get_optional() const;
	void set_not(const bool & _not);
	bool get_not() const;


	UnitType get_unit_type_() const;
	const char* get_name() const;
	Word::WordType get_word_type() const;
	Delimiter::DelimiterType get_delimiter_type() const;
	String::StringType get_string_type() const;
	const TreeBuilderNode* get_t_builder() const;

	bool is_good_token(Token* token) const;

};

