#pragma once
#include "stdafx.h"
#include <list>

namespace IR
{
	class TempTable
	{
	private:
		list<OperandType::Variable*> free_temp_;
		SymbolTable* symbol_table_;
		OperandType::Variable* new_temp();
	public:
		TempTable(SymbolTable* symbol_table);
		OperandType::Variable* lend();
		void revert(OperandType::Variable*);
	};
}