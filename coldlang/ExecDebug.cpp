#include "stdafx.h"
#include "ExecCompiler.h"
#include "ExecDebug.h"

namespace Compile {

	ExecDebug::ExecDebug(const ExecCompiler * exec_compiler)
		: exec_compiler(exec_compiler)
	{
		
	}

	void ExecDebug::PrintReg(wstring str, X86Gp & reg)
	{
		X86Reg _reg = reg;
		if (_reg.isGpq())
		{
			CCFuncCall* call = exec_compiler->compiler
				->call((size_t)Print, FuncSignature2<int, const char*, int64_t>());
			// call->setArg(0, imm((size_t)str.c_str()));
			call->setArg(0, imm((size_t)L""));
			call->setArg(1, _reg);
		}
	}

	void ExecDebug::Print(const wchar_t * info, int64_t i)
	{
		wcout << i << endl;
	}
}
