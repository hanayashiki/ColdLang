#pragma once

namespace Compile
{
	class ExecCompiler;

	class ExecDebug
	{
	private:
		const ExecCompiler * exec_compiler;
		static void Print(const wchar_t * info, int64_t);
	public:
		explicit ExecDebug(const ExecCompiler * exec_compiler);
		void PrintReg(wstring str, X86Gp & reg);

		static wstring to_string(const Operand & operand);
		static wstring to_string(const X86Mem &);
		static wstring to_string(const X86Gp &);
	};
}
