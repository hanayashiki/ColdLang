#include "stdafx.h"
#include "ExecCompiler.h"
#include "ValueTypeVec.h"
#include "Code.h"

#include "BlockEnv.h"
#include "CompilerUtils.h"
#include "BytecodeTyper.h"

#include "BlockResult.h"

#include "log.h"

#include <stddef.h>

using namespace asmjit;
using CldRuntime::BlockEnv;

namespace Compile {
	JitRuntime ExecCompiler::jitRuntime;
	bool ExecCompiler::isHandlersInited = false;

	ExecCompiler::ExecCompiler(IR::SymbolToType * symbolToType)
		: logger(stderr), debug(this), symbolToType(symbolToType),
		operands(
		{ 
			&Code::RuntimeStackPtr,
			&Code::BlockResultPtr,
			&Code::NContexts,
			&Code::ContextsPtr
		}, 
		{
			BlockEnv::runtime_stack_reg,
			BlockEnv::block_result_reg,
			BlockEnv::n_contexts_reg,
			BlockEnv::contexts_reg
		})
	{
		code.init(jitRuntime.getCodeInfo());
		code.setLogger(&logger);
		compiler = std::make_shared<X86Compiler>(&code);
		compiler->addFunc(Code::AsmjitFuncSignature);

		integerEmitter = std::make_unique<ExecIntegerEmitter>(compiler);
		
		if (!isHandlersInited) {
			InitQuadHandlers();
			isHandlersInited = true;
		}
	}

	vector<asmjit::Operand> ExecCompiler::Locate(const initializer_list<OperandParams> & symbols)
	{
		// TODO: decide to use reg or mem
		vector<asmjit::Operand> usedOperands;
		usedOperands.reserve(symbols.size());

		set<OperandParams> symbolsAsVars =
			CompilerUtils::Select(symbols, std::function<bool(OperandParams)>([](OperandParams params) {
				return dynamic_cast<Variable*>(params.symbol) != nullptr;
			}));

		set<const Variable*> variables;
		for (auto & t : symbolsAsVars) 
		{
			variables.emplace(dynamic_cast<const Variable*>(t.symbol));
		}

		for (auto & params : symbols) 
		{
			bool loadValue = params.loadValue;
			if (Variable* var = dynamic_cast<Variable*>(params.symbol)) 
			{
				usedOperands.push_back(Locate(var, loadValue, params.valueType, variables));
			} 
			else if (Constant* constant = dynamic_cast<Constant*>(params.symbol)) 
			{
				usedOperands.push_back(Locate(constant));
			}
			else 
			{
				assert(false);
			}
		}

		return usedOperands;
	}

	asmjit::Operand ExecCompiler::Locate(Variable * var, bool loadVal, ValueType type, const set<const Variable*>& varGroup)
	{
		// TODO: Mem alternative
		CLD_DEBUG << L"Locating variable: " << to_string(var) << std::endl;
		if (operands.InReg(var)) {
			return operands.GetReg(var);
		}
		else {
			bool success = operands.ToReg(var);
			CLD_DEBUG << L"success == " << success << std::endl;
			if (!success) {
				const Variable * spilled = operands.GetInRegVariable(varGroup);
				X86Mem spilledMem = operands.GenerateMem(spilled);
				X86Gp spilledReg = operands.GetReg(spilled);
				MoveRegToMem(spilledMem, spilledReg, GetType(spilled));
				operands.ReleaseVariable(spilled);
				operands.AllocVariable(var, spilledReg);
			}
			CLD_DEBUG << operands.DumpVariableToOperand() << std::endl;
			X86Gp reg = operands.GetReg(var);
			if (loadVal) {
				X86Mem base = operands.GenerateMem(var);
				MoveMemToReg(reg, base, type);
			}
			return reg;
		}
	}

	asmjit::Operand ExecCompiler::Locate(Constant * constant)
	{
		RuntimeValue * value = constant->get_value();
		ValueType valueType = value->type;
		switch (valueType) {
		case AnyVal:
			assert(false);
			break;
		case IntegerVal:
			return asmjit::imm(static_cast<IntegerValue*>(value)->value);
		case FloatVal:
			assert(false);
			break;
		case BooleanVal:
			return asmjit::imm(static_cast<BooleanValue*>(value)->value);
		case NoneVal:
			return asmjit::imm(0);
		default:
			assert(false);
		}
	}

	void ExecCompiler::MoveMemToReg(X86Gp reg, X86Mem base, ValueType valueType)
	{
		switch (valueType) {
		case AnyVal:
			assert(false);
			break;
		case IntegerVal:
			compiler->mov(reg, WithOffset(base, offsetof(IntegerValue, value)));
			break;
		case FloatVal:
			assert(false);
			break;
		case BooleanVal:
			compiler->mov(reg, WithOffset(base, offsetof(IntegerValue, value)));
			compiler->mov(reg, base);
		case NoneVal:
			compiler->mov(reg, 0);
			break;
		default:
			assert(false);
		}
	}

	void ExecCompiler::MoveRegToMem(X86Mem base, X86Gp reg, ValueType valueType)
	{
		compiler->mov(WithOffset(base, offsetof(IntegerValue, type)), asmjit::imm_u(valueType));
		switch (valueType) {
		case AnyVal:
			assert(false);
			break;
		case IntegerVal:
			compiler->mov(WithOffset(base, offsetof(IntegerValue, value)), reg);
			break;
		case FloatVal:
			assert(false);
			break;
		case BooleanVal:
			compiler->mov(WithOffset(base, offsetof(BooleanValue, value)), reg);
		case NoneVal:
			compiler->mov(reg, 0);
			break;
		default:
			assert(false);
		}
	}

	ValueType ExecCompiler::GetType(const Symbol * symbol)
	{
		if (auto var = dynamic_cast<const Variable*>(symbol)) {
			return symbolToType->at(var);
		}
		else if (auto constant = dynamic_cast<const Constant*>(symbol)) {
			return constant->get_value()->type;
		}
		// todo!
		else if (auto literal = dynamic_cast<const Literal*>(symbol)) {
			assert(false);
			return ErrorVal;
		}
		else {
			assert(false);
			return ErrorVal;
		}
	}

	X86Mem ExecCompiler::WithOffset(X86Mem mem, int64_t offset)
	{
		mem.addOffset(offset);
		return mem;
	}

	void ExecCompiler::CompileBinaryImpl(Symbol * target, OpType opType,
		ValueType leftType, Symbol * leftSymbol, ValueType rightType, Symbol * rightSymbol)
	{
		CLD_DEBUG << operands.DumpVariableToOperand() << std::endl;
		QuadHandlers[opType][leftType][rightType](
			this, target, leftSymbol, rightSymbol
		);
	}

	void ExecCompiler::CompileUnaryImpl(Symbol * target, OpType op_type, 
		ValueType type, Symbol * source)
	{

	}

	void ExecCompiler::CompileSingleImpl(Symbol * target, BytecodeEnum bytecode_name, ValueType source_type)
	{
		switch (bytecode_name) {
		case EnumRetAcc:
		{
			vector<Operand> op = Locate({
				{ target, true, source_type },
				{ &Code::BlockResultPtr, true, ObjectPointerVal }
			});
			// TOOD: fix this
			X86Gp valueReg = op[0].as<X86Gp>();
			X86Gp returnValuePtrBaseReg = op[1].as<X86Gp>();
			X86Mem returnValuePtr = asmjit::x86::ptr(returnValuePtrBaseReg,
				offsetof(BlockResult, return_value));
			break;
		}
		default:
			std::wcout << "Not implemented" << std::endl;
			assert(false);
		}
	}

	Code ExecCompiler::GetCode()
	{
		Code::FuncPtr func_ptr;
		compiler->ret();

		compiler->endFunc();
		Error err = compiler->finalize();
		if (err) 
		{
			CLD_FATAL << "Asmjit error code: " << err << ": "
				<< asmjit::DebugUtils::errorAsString(err) << std::endl;
			assert(false);
		}
		jitRuntime.add(&func_ptr, &code);
		return Code(func_ptr);
	}
}
