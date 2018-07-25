#include "stdafx.h"
#include "NativeSymbol.h"

namespace IR{
	size_t SymbolTable::symbol_id = 0;

	SymbolTable::SymbolTable(SymbolTable * parent) : parent_(parent)
	{
	}

	void SymbolTable::add(OperandType::Variable * variable)
	{
		add_symbol(variable->get_name(), variable);
	}

	void SymbolTable::add(OperandType::NativeSymbol * native)
	{
		add_symbol(native->get_name(), native);
	}

	void SymbolTable::add_symbol(const wchar_t* name, Symbol * symbol)
	{
		name_to_symbol_.insert(std::make_pair(name, symbol));
		symbol->set_id(symbol_id);
		id_to_symbol_.insert(std::make_pair(symbol->get_id(), symbol));
		symbol_id++;
	}

	bool SymbolTable::define(OperandType::Variable * symbol)
	{
		if (name_to_symbol_.find(symbol->get_name()) != name_to_symbol_.end())
		{
			return false;
		} else
		{
			add(symbol);
			return true;
		}
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
		if (iter != name_to_symbol_.end()) 
		{
			return iter->second;
		} 
		else
		{
			return nullptr;
		}
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