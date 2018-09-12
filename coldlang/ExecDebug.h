#pragma once

#include "../asmjit/asmjit.h"
#include <cstdio>

#define CHECK ExecDebug::ErrorCollector::instance << 

namespace Compile
{
	class ExecCompiler;

	class ExecDebug
	{
	private:
		ExecCompiler * exec_compiler;
		static void Print(const wchar_t * info, int64_t);
	public:
		explicit ExecDebug(ExecCompiler * exec_compiler);
		void PrintReg(wstring str, const X86Gp & reg);

		static wstring to_string(const Operand & operand);
		static wstring to_string(const X86Mem &);
		static wstring to_string(const X86Gp &);

		class ErrorCollector {
		public:
			static ErrorCollector instance;
			ErrorCollector & operator << (asmjit::Error err);
		};
	};
}
