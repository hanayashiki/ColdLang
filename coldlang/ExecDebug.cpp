#include "stdafx.h"
#include "ExecCompiler.h"
#include "ExecDebug.h"

#include "log.h"

namespace Compile {

	ExecDebug::ExecDebug(ExecCompiler * exec_compiler)
		: exec_compiler(exec_compiler)
	{
	}

	void ExecDebug::PrintReg(wstring str, const X86Gp & reg)
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
		CLD_DEBUG << hex << i << dec << endl;
	}

	wstring ExecDebug::to_string(const Operand & operand)
	{
		Operand op = operand;
		if (op.isMem()) {
			return to_string(op.as<X86Mem>());
		}
		else if (op.isReg()) {
			return to_string(op.as<X86Gp>());
		}
		else if (op.isImm()) {
			return std::to_wstring(op.as<Imm>().getInt64());
		}
		else {
			assert(false);
		}
	}

	wstring ExecDebug::to_string(const X86Mem & mem)
	{
		return L"Mem";
	}

	wstring ExecDebug::to_string(const X86Gp & gp)
	{
		return L"Gp";
	}

	ExecDebug::ErrorCollector ExecDebug::ErrorCollector::instance;

	ExecDebug::ErrorCollector & ExecDebug::ErrorCollector::operator<<(asmjit::Error err)
	{
		if (err) {
			CLD_FATAL << "Asmjit error code: " << err << ": "
				<< asmjit::DebugUtils::errorAsString(err) << std::endl;
			assert(false);
		}
		return *this;
	}
}
