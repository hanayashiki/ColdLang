#include "stdafx.h"
#include "ExecIntegerEmitter.h"
#include "ExecOperands.h"

#include "ExecDebug.h"

#define MAKE_EMITTER(block) [=](std::shared_ptr<X86Compiler>& cc){block}

using namespace asmjit;

namespace Compile {
	ExecIntegerEmitter::ExecIntegerEmitter(std::shared_ptr<X86Compiler>& compiler)
		: compiler(compiler)
	{
	}

	void ExecIntegerEmitter::Emit(asmjit::Operand target, OpType op_type, 
		asmjit::Operand left, asmjit::Operand right)
	{
		using namespace IR::BytecodeClass;
		switch (op_type) {
		case OpAdd:
		case OpSub:
		case OpMove:
			EmitType0(target, op_type, left, right);
			break;
		}
	}

	asmjit::Operand ExecIntegerEmitter::RegOrTemp(asmjit::Operand operand, asmjit::X86Gp reg)
	{
		return operand.isReg() ? operand : reg;
	}

	asmjit::Operand ExecIntegerEmitter::RegOrTemp0(asmjit::Operand operand)
	{
		return RegOrTemp(operand, ExecOperands::temp_reg_0);
	}

	asmjit::Operand ExecIntegerEmitter::RegOrTemp1(asmjit::Operand operand)
	{
		return operand.isReg() ? operand : asmjit::Operand(ExecOperands::temp_reg_1);
	}

	std::shared_ptr<ExecIntegerEmitter::OperandAdaptor> 
		ExecIntegerEmitter::GetAdaptor(const initializer_list<asmjit::Operand> & from, const initializer_list<asmjit::Operand> & to)
	{
		return std::shared_ptr<ExecIntegerEmitter::OperandAdaptor>(
			new OperandAdaptor(from, to)
		);
	}

	const ExecIntegerEmitter::InstrEmitter & ExecIntegerEmitter::SelectBestEmitter(const vector<InstrEmitter> & emitters)
	{
		const InstrEmitter * best = &emitters.at(0);
		for (const auto & emitter : emitters) {
			if (emitter.adaptor->Size() < best->adaptor->Size()) {
				best = &emitter;
			}
		}
		return *best;
	}

	void ExecIntegerEmitter::EmitType0(asmjit::Operand target, IR::BytecodeClass::OpType op_type,
		asmjit::Operand left, asmjit::Operand right)
	{
		using namespace IR::BytecodeClass;

		vector<InstrEmitter> emitters;

		switch (op_type) {
		case OpAdd:
			emitters = vector<InstrEmitter>({
				{
					// ASMJIT_INST_2x(add, Add, X86Gp, X86Gp)
					target.isReg(),
					GetAdaptor({ left, right }, { target, RegOrTemp1(right) }),
					MAKE_EMITTER({ CHECK cc->add(target.as<X86Gp>(), RegOrTemp1(right).as<X86Gp>()); })
				},
				{
					// ASMJIT_INST_2x(add, Add, X86Gp, X86Mem)
					target.isReg() && right.isMem(),
					GetAdaptor({ left }, { target }),
					MAKE_EMITTER({ CHECK cc->add(target.as<X86Gp>(), right.as<X86Mem>()); })
				},
				{
					// ASMJIT_INST_2i(add, Add, X86Gp, Imm)
					target.isReg() && right.isImm(),
					GetAdaptor({ left }, { target }),
					MAKE_EMITTER({ CHECK cc->add(target.as<X86Gp>(), right.as<Imm>()); })
				},
				{
					// ASMJIT_INST_2x(add, Add, X86Mem, X86Gp)
					target.isMem(),
					GetAdaptor({ left, right }, { target, RegOrTemp1(right) }),
					MAKE_EMITTER({ CHECK cc->add(target.as<X86Mem>(), RegOrTemp1(right).as<X86Gp>()); })
				},
				{
					// ASMJIT_INST_2i(add, Add, X86Mem, Imm)
					target.isMem() && right.isImm(),
					GetAdaptor({ left }, { target }),
					MAKE_EMITTER({ CHECK cc->add(target.as<X86Mem>(), right.as<Imm>()); })
				},
			});
			break;
		case OpSub:
			emitters = vector<InstrEmitter>({
				{
					// ASMJIT_INST_2x(~, ~~, X86Gp, X86Gp)
					target.isReg(),
					GetAdaptor({ left, right }, { target, RegOrTemp1(right) }),
					MAKE_EMITTER({ CHECK cc->sub(target.as<X86Gp>(), RegOrTemp1(right).as<X86Gp>()); })
				},
				{
					// ASMJIT_INST_2x(~, ~~, X86Gp, X86Mem)
					target.isReg() && right.isMem(),
					GetAdaptor({ left }, { target }),
					MAKE_EMITTER({ CHECK cc->sub(target.as<X86Gp>(), right.as<X86Mem>()); })
				},
				{
					// ASMJIT_INST_2i(~, ~~, X86Gp, Imm)
					target.isReg() && right.isImm(),
					GetAdaptor({ left }, { target }),
					MAKE_EMITTER({ CHECK cc->sub(target.as<X86Gp>(), right.as<Imm>()); })
				},
				{
					// ASMJIT_INST_2x(~, ~~, X86Mem, X86Gp)
					target.isMem(),
					GetAdaptor({ left, right }, { target, RegOrTemp1(right) }),
					MAKE_EMITTER({ CHECK cc->sub(target.as<X86Mem>(), RegOrTemp1(right).as<X86Gp>()); })
				},
				{
					// ASMJIT_INST_2i(~, ~~, X86Mem, Imm)
					target.isMem() && right.isImm(),
					GetAdaptor({ left }, { target }),
					MAKE_EMITTER({ CHECK cc->sub(target.as<X86Mem>(), right.as<Imm>()); })
				},
			});
			break;
		case OpMove:
			emitters = vector<InstrEmitter>({
				{
					// ASMJIT_INST_2x(~, ~~, X86Gp, X86Gp)
					target.isReg(),
					GetAdaptor({ right }, { RegOrTemp1(right) }),
					MAKE_EMITTER({ CHECK cc->mov(target.as<X86Gp>(), RegOrTemp1(right).as<X86Gp>()); })
				},
				{
					// ASMJIT_INST_2x(~, ~~, X86Gp, X86Mem)
					target.isReg() && right.isMem(),
					GetAdaptor({ }, { }),
					MAKE_EMITTER({ CHECK cc->mov(target.as<X86Gp>(), right.as<X86Mem>()); })
				},
				{
					// ASMJIT_INST_2i(~, ~~, X86Gp, Imm)
					target.isReg() && right.isImm(),
					GetAdaptor({ }, { }),
					MAKE_EMITTER({ CHECK cc->mov(target.as<X86Gp>(), right.as<Imm>()); })
				},
				{
					// ASMJIT_INST_2x(~, ~~, X86Mem, X86Gp)
					target.isMem(),
					GetAdaptor({ }, { }),
					MAKE_EMITTER({ CHECK cc->mov(target.as<X86Mem>(), RegOrTemp1(right).as<X86Gp>()); })
				},
				{
					// ASMJIT_INST_2i(~, ~~, X86Mem, Imm)
					target.isMem() && right.isImm(),
					GetAdaptor({ }, { }),
					MAKE_EMITTER({ CHECK cc->mov(target.as<X86Mem>(), right.as<Imm>()); })
				},
			});
			break;
		}

		const InstrEmitter & best = SelectBestEmitter(emitters);
		best.Exec(compiler);
	}


	ExecIntegerEmitter::OperandAdaptor::
		OperandAdaptor(const initializer_list<asmjit::Operand> & from, 
			const initializer_list<asmjit::Operand> & to)
	{
		assert(from.size() == to.size());
		for (auto from_iter = from.begin(), to_iter = to.begin();
			from_iter != from.end() && to_iter != to.end();
			from_iter++, to_iter++) 
		{
			if (*from_iter != *to_iter) 
			{

#define CASE_OPTYPE_TO_INSTR(to_type_getter, to_type, from_type_getter, from_type) \
	if (to_iter->##to_type_getter##() && from_iter->##from_type_getter##()) \
	{ \
		emitter.push_back([=](std::shared_ptr<X86Compiler>& compiler) \
		{ \
			compiler->mov(to_iter->as<to_type>(), from_iter->as<from_type>()); \
		}); \
		continue; \
	}

			CASE_OPTYPE_TO_INSTR(isReg, asmjit::X86Gp, isReg, asmjit::X86Gp);
			CASE_OPTYPE_TO_INSTR(isReg, asmjit::X86Gp, isMem, asmjit::X86Mem);
			CASE_OPTYPE_TO_INSTR(isReg, asmjit::X86Gp, isImm, asmjit::Imm);
			CASE_OPTYPE_TO_INSTR(isMem, asmjit::X86Mem, isReg, asmjit::X86Gp);
			CASE_OPTYPE_TO_INSTR(isMem, asmjit::X86Mem, isImm, asmjit::Imm);

			if (to_iter->isMem() && from_iter->isMem()) 
			{
				emitter.push_back([=](std::shared_ptr<X86Compiler>& compiler) 
				{
					compiler->mov(ExecOperands::temp_reg_0, from_iter->as<X86Mem>());
				});
				emitter.push_back([=](std::shared_ptr<X86Compiler>& compiler) 
				{
					compiler->mov(to_iter->as<X86Mem>(), ExecOperands::temp_reg_0);
				});
				continue;
			}

#undef CASE_OPTYPE_TO_INSTR

			}
		}
	}
}