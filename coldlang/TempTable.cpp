#include "stdafx.h"
#include "TempTable.h"

namespace IR {

	TempTable::TempTable(SymbolTable * symbol_table)
		: symbol_table_(symbol_table), id_(0)
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
			ret = new_temp();
			ret->set_temp(true);
		}
		return ret;
	}

	void TempTable::revert(OperandType::Variable * temp)
	{
		free_temp_.push_front(temp);
	}

	OperandType::Variable * TempTable::new_temp()
	{
		wstring new_temp_name = L"%t" + to_wstring(id_++);
		symbol_table_->mock({ new_temp_name.c_str() });
		return static_cast<OperandType::Variable*>(symbol_table_->get_by_name(new_temp_name.c_str()));
	}
}
