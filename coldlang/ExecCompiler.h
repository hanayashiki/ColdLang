#pragma once
#include "Compiler.h"
#include "../asmjit/asmjit.h"
#include "Code.h"
#include "ExecDebug.h"

namespace Compile
{

	class ExecCompiler : public Compiler
	{
	public:
		ExecCompiler();
		virtual Code GetCode() override;
	private:
		X86Gp & GetTempReg64();
		void RevertTempReg64(const X86Gp & gp);

		friend class Code;
		friend class ExecDebug;

		ExecDebug debug;

		static JitRuntime jit_runtime_;

		CodeHolder code;
		FileLogger logger;
		shared_ptr<X86Compiler> compiler;

		unordered_map<Variable*, X86Gp> variable_to_reg;

		list<X86Gp> temp_reg64;

		X86Gp & IntegerReg(Variable*);
		void AddCodeIntegerTwoOperands(Symbol * target, OpType op_type, Symbol * left_symbol, Symbol * right_symbol);
		void AddCodeIntegerRegReg(OpType op_type, X86Gp & reg1, X86Gp & reg2) const;
		void AddCodeIntegerRegImm(OpType op_type, X86Gp & reg, Imm && imm) const;

		void AddCodeIntegerOneOperand(Symbol * target, OpType op_type, Symbol * source);
	protected:
		static const bool executable = true;
		virtual void CompileBinaryImpl(Symbol * target, OpType op_type,
			ValueType left_type, Symbol * left_symbol,
			ValueType right_type, Symbol * right_symbol) override;
		virtual void CompileUnaryImpl(Symbol * target, OpType op_type,
			ValueType type, Symbol* source) override;
	};
}
