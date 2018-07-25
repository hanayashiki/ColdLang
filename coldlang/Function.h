#pragma once
#include "stdafx.h"
#include "Variable.h"

namespace IR
{
	class Function : public CLObject
	{
	private:
		shared_ptr<Token> token_;
		vector<OperandType::Variable *> parameter_list_;
	public:
		Function(shared_ptr<Token> & token, vector<OperandType::Variable *> & parameter_list);
		wstring to_string();
	};
};