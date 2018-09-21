#pragma once
#include "../asmjit/asmjit.h"
#include "BytecodeClass.h"
#include <utility>
#include <functional>
#include <initializer_list>

namespace Compile
{
	class ExecIntegerEmitter {
	protected:
		typedef asmjit::X86Compiler X86Compiler;
		typedef IR::BytecodeClass::OpType OpType;
	public:
		ExecIntegerEmitter(std::shared_ptr<X86Compiler> & compiler);
		// Emits x86 instructions, allows 
		//	(Reg|Mem) <- (Reg|Mem) op (Reg|Mem|Imm)
		void Emit(asmjit::Operand target, IR::BytecodeClass::OpType op_type, asmjit::Operand left, asmjit::Operand right);
	private:
		class OperandAdaptor {
		public:
			OperandAdaptor(const initializer_list<asmjit::Operand> & from, 
				const initializer_list<asmjit::Operand> & to);
			size_t Size() { return emitter.size(); };
			void Exec(std::shared_ptr<X86Compiler>& c) { 
				for (auto & e : emitter) {
					e(c); 
				} 
			}
		private:
			vector<function<void(std::shared_ptr<X86Compiler>&)>> emitter;
		};

		struct InstrEmitter {
			bool condition;
			std::shared_ptr<OperandAdaptor> adaptor;
			function<void(std::shared_ptr<X86Compiler>& c)> emitter;
			void Exec(std::shared_ptr<X86Compiler>& c) const {
				adaptor->Exec(c);
				emitter(c);
			}
		};

		std::shared_ptr<X86Compiler> compiler;

		asmjit::Operand RegOrTemp(asmjit::Operand operand, asmjit::X86Gp reg);
		asmjit::Operand RegOrTemp0(asmjit::Operand operand);
		asmjit::Operand RegOrTemp1(asmjit::Operand operand);

		std::shared_ptr<ExecIntegerEmitter::OperandAdaptor> GetAdaptor(const initializer_list<asmjit::Operand> & from, const initializer_list<asmjit::Operand> & to);
		const InstrEmitter & SelectBestEmitter(const vector<InstrEmitter> & emitters);
		void EmitType0(asmjit::Operand target, IR::BytecodeClass::OpType op_type, asmjit::Operand left, asmjit::Operand right);
	};
}