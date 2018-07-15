#include "stdafx.h"
#include "TempTable.h"

namespace IR {

	TempTable::TempTable(SymbolTable * symbol_table)
		: symbol_table_(symbol_table)
	{
	}

	OperandType::Variable * TempTable::lend()
	{
		OperandType::Variable * ret;
		if (free_temp_.size() > 0)
		{
			ret = free_temp_.front();
			free_temp_.pop_front();
		} else
		{
			//ret = new OperandType::Variable();
		}
		return nullptr;
	}

	void TempTable::revert(OperandType::Variable * temp)
	{
		free_temp_.push_front(temp);
	}

	OperandType::Variable * TempTable::new_temp()
	{
		static int id = 0;
		//return new OperandType::Variable()
		return nullptr;
	}
}
