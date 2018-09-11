#pragma once
#include "../asmjit/asmjit.h"
#include "BlockEnv.h"
#include "Symbol.h"
#include "Variable.h"

#include <set>

#define X86 asmjit::x86

template<>
struct std::less<asmjit::X86Gp> {
	bool operator()(const asmjit::X86Gp& lhs, const asmjit::X86Gp& rhs) const {
		return lhs.getSignature() < rhs.getSignature() || (lhs.getSignature() == rhs.getSignature()) && (lhs.getId() < rhs.getId());
	}
};

namespace Compile {
	class ExecOperands {
	private:
		typedef asmjit::Operand Operand;
		typedef asmjit::X86Gp X86Gp;
		typedef asmjit::X86Gpq X86Gpq;

		typedef IR::OperandType::Symbol Symbol;
		typedef IR::OperandType::Variable Variable;

		inline const static size_t GPR_COUNT = X86Gpq::kIdR15 + 1;

		inline const static X86Gp PlaceHolder = asmjit::X86Gp();

	public:
		inline const static asmjit::X86Gp temp_reg_0 = asmjit::x86::r10;
		inline const static asmjit::X86Gp temp_reg_1 = asmjit::x86::r11;

		typedef std::function<void(const shared_ptr<asmjit::X86Compiler> &)> LoadHelper;

		/* GP usages */
		/* according to:
		 * https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/x64-architecture 
		 */
		
		const static vector<asmjit::X86Gp> NonVolatileGps;
		const static vector<asmjit::X86Gp> VolatileGps;
		const static set<asmjit::X86Gp> InitialFreeGps;


		ExecOperands(const initializer_list<Variable*> & arguments,
			const initializer_list<X86Gp> & locations);
		size_t GetRuntimeStackSize() { return runtimeStackSize; }
		~ExecOperands();

		bool ToReg(const Variable* var); // Load var from mem to reg
		bool ToMem(const Variable* var); // Release the reg and load to mem
		bool InReg(const Variable* var);
		bool InMem(const Variable* var);
		X86Mem GetMem(const Variable* var);
		X86Gp GetReg(const Variable* var);
		X86Mem GenerateMem(const Variable * var);

		void ReleaseVariable(const Variable * var);
		const Variable* GetInRegVariable(const set<const Variable*> & excluding);
		bool AllocVariable(const Variable * var, const X86Gp & gp);

		wstring DumpVariableToOperand() { return to_string(VariableToOperand); }

	private:
		std::unordered_map<const Variable*, Operand> VariableToOperand;
		
		wstring to_string(const std::unordered_map<const Variable*, Operand> &) const;

		std::set<const Variable*> InRegVariables;
		// std::set<Variable*> InMemVariables;
		std::set<X86Gp> UsedGps;
		std::set<X86Gp> FreeGps;

		void ReleaseReg(const X86Gp & gp);
		void AllocReg(const X86Gp & gp);
		bool IsFree(const X86Gp & gp);

		const inline static X86Gp runtimeStack = 
			CldRuntime::BlockEnv::runtime_stack_reg;
		size_t runtimeStackSize = 0;
	};
}

#undef X86