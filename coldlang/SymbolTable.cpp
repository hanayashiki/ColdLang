#include "stdafx.h"
#include "NativeSymbol.h"
#include "Code.h"
#include "log.h"

namespace IR{
	size_t SymbolTable::symbol_id = 0;

	SymbolTable::SymbolTable(SymbolTable * parent) : parent_(parent), var_id(0)
	{
		if (parent == nullptr) 
		{
			nest_level_ = 0;
		}
		else 
		{
			nest_level_ = parent->nest_level_ + 1;
		}
		init_native_symbols();
	}

	void SymbolTable::add(OperandType::Variable * variable)
	{
		add_symbol(variable->get_name(), variable);
		variable->set_var_id(var_id);
		var_id++;
	}

	void SymbolTable::add(OperandType::NativeSymbol * native)
	{
		add_symbol(native->get_name(), native);
	}

	void SymbolTable::add_symbol(const wchar_t* name, Symbol * symbol)
	{
		name_to_symbol_.insert(std::make_pair(name, symbol));
		symbol->set_id(symbol_id);
		symbol->set_nest_level(nest_level_);
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

	size_t SymbolTable::add_sub_symbol_table()
	{
		size_t id = sub_symbol_tables_.size();
		sub_symbol_tables_.push_back(new SymbolTable(this));
		return id;
	}

	SymbolTable * SymbolTable::get_sub_symbol_table(size_t id)
	{
		return sub_symbol_tables_.at(id);
	}

	SymbolTable * SymbolTable::get_parent_symbol_table()
	{
		return parent_;
	}

	OperandType::Symbol * IR::SymbolTable::get_by_name(const wchar_t * name)
	{
		auto iter = name_to_symbol_.find(name);
		if (iter != name_to_symbol_.end()) 
		{
			return iter->second;
		} 
		else if (parent_ == nullptr) {
			return nullptr;
		} else
		{
			return parent_->get_by_name(name);
		}
	}

	OperandType::Symbol* SymbolTable::get_by_id(size_t id)
	{
		auto iter = id_to_symbol_.find(id);
		return iter->second;
	}

	wstring SymbolTable::dump_to_string(size_t space_offset)
	{
		wstring ret;
		for (auto pair : name_to_symbol_)
		{
			for (int i = 0; i < 4 * space_offset; i++) ret += L" ";
			ret += pair.first;
			if (auto var = dynamic_cast<Variable*>(pair.second)) {
				ret += L"[" + to_wstring(var->get_var_id()) + L"]";
			}
			ret += L"\n";
		}
		for (auto symbol_table : sub_symbol_tables_)
		{
			ret += symbol_table->dump_to_string(space_offset + 1);
		}
		return ret;
	}

	SymbolTable::~SymbolTable()
	{
		for (auto pair : name_to_symbol_)
		{
			Symbol * symbol = pair.second;
			if (!symbol->is_static()) {
				CLD_DEBUG << "delete " << to_string(symbol) << std::endl;
				delete pair.second;
			}
		}
		for (auto symbol_table : sub_symbol_tables_)
		{
			delete symbol_table;
		}
	}

	void SymbolTable::init_native_symbols()
	{
		add(dynamic_cast<Variable*>(Symbol::Acc));
		add(&Compile::Code::RuntimeStackPtr);
		add(&Compile::Code::BlockResultPtr);
		add(&Compile::Code::NContexts);
		add(&Compile::Code::ContextsPtr);
	}

}