#include "stdafx.h"
#include "FunctionTable.h"

namespace IR
{
	Function::Function(shared_ptr<Token>& token, vector<Variable*>& parameter_list)
		: token_(token), parameter_list_(parameter_list)
	{
	}

	wstring IR::Function::to_string()
	{
		return L"function";
	}

}