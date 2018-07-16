#include "stdafx.h"

namespace IR{
	size_t SymbolTable::symbol_id = 0;

	SymbolTable::SymbolTable(SymbolTable * parent) : parent_(parent)
	{
	}

	void SymbolTable::add(OperandType::Variable * variable)
	{
		name_to_symbol_.insert(std::make_pair(variable->get_name(), variable));
		id_to_symbol_.insert(std::make_pair(variable->get_id(), variable));
		variable->set_id(symbol_id);
		symbol_id++;
	}

	void SymbolTable::mock(initializer_list<const wchar_t *> init_list)
	{
		for (auto name : init_list)
		{
			add(new IR::OperandType::Variable(Word::mock(name)));
		}
	}

	OperandType::Symbol * IR::SymbolTable::get_by_name(const wchar_t * name)
	{
		auto iter = name_to_symbol_.find(name);
		return iter->second;
	}

	OperandType::Symbol* SymbolTable::get_by_id(size_t id)
	{
		auto iter = id_to_symbol_.find(id);
		return iter->second;
	}

	SymbolTable::~SymbolTable()
	{
		for (auto pair : name_to_symbol_)
		{
			// wcout << "~SymbolTable token use count: " << pair.second->get_token().use_count() << endl;
			delete pair.second;
		}

	}

}