#pragma once
#include "stdafx.h"
#include <list>

namespace IR
{
	class SymbolTable;

	class TempTable
	{
	private:
		int id_;
		list<OperandType::Variable*> free_temp_;
		SymbolTable* symbol_table_;
		OperandType::Variable* new_temp();
	public:
		explicit TempTable(SymbolTable* symbol_table);
		OperandType::Variable* lend();
		void revert(OperandType::Variable*);
	};
}