#include "stdafx.h"
#include "ExecCompiler.h"
#include "ValueTypeVec.h"
#include "Code.h"

#include "BlockEnv.h"
#include "CompilerUtils.h"
#include "BytecodeTyper.h"

#include "BlockResult.h"

#include "log.h"
#include "utils.h"

#include <stddef.h>

using namespace asmjit;
using CldRuntime::BlockEnv;

#define OFFSET_SIZE(s, m) offsetof(s, m), sizeof(s::m)

namespace Compile 
{
	JitRuntime ExecCompiler::jitRuntime;
	bool ExecCompiler::isHandlersInited = false;

	ExecCompiler::ExecCompiler(IR::SymbolToType * symbolToType)
		: logger(stderr), debug(this), symbolToType(symbolToType),
		operands(
		{ 
			&Code::BlockResultPtr,
			&Code::NContexts,
			&Code::ContextsPtr
		}, 
		{
			BlockEnv::block_result_reg,
			BlockEnv::n_contexts_reg,
			BlockEnv::contexts_reg
		})
	{
		code.init(jitRuntime.getCodeInfo());
		code.setLogger(&logger);
		compiler = std::make_shared<X86Compiler>(&code);
		compiler->addFunc(Code::AsmjitFuncSignature);

		Def(BlockEnv::runtime_stack_reg);
		Def(BlockEnv::block_result_reg);
		Def(BlockEnv::n_contexts_reg);
		Def(BlockEnv::contexts_reg);

		// debug.PrintReg(L"block_result_reg", BlockEnv::block_result_reg);

		prologInsertingPoint = compiler->getCursor();

		integerEmitter = std::make_unique<ExecIntegerEmitter>(compiler);
		stackManager = std::make_unique<ExecStack>(this);

		if (!isHandlersInited) {
			InitQuadHandlers();
			isHandlersInited = true;
		}
	}

	CBNode * ExecCompiler::DummyNode()
	{
		Comment("Dummy");
		return compiler->getCursor();
	}

	X86Gp & ExecCompiler::Def(X86Gp & gp)
	{
		operands.RecordGpUse(gp);
		return gp;
	}

	const X86Gp & ExecCompiler::Def(const X86Gp & gp)
	{
		operands.RecordGpUse(gp);
		return gp;
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
			else if (Literal* literal = dynamic_cast<Literal*>(params.symbol))
			{
				usedOperands.push_back(Locate(literal));
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

		Comment("Locate %ls", to_string(var).c_str());

		if (operands.InReg(var)) {
			return operands.GetReg(var);
		}
		else {
			bool success = operands.ToReg(var);
			if (!success) {
				const Variable * spilled = operands.GetInRegVariable(varGroup);
				const X86Gp spilledReg = Reserve(spilled);
				operands.AllocVariable(var, spilledReg);
			}
			X86Gp reg = operands.GetReg(var);
			if (loadVal) {
				X86Mem base = operands.GenerateMem(var);
				MoveMemToReg(Def(reg), base, type);
			}
			return reg;
		}
	}

	asmjit::Operand ExecCompiler::Locate(Literal * literal)
	{
		RuntimeValue * value = literal->get_value();
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

	const X86Gp ExecCompiler::Reserve(const Variable * var)
	{
		X86Mem mem = operands.GenerateMem(var);
		X86Gp reg = operands.GetReg(var);
		MoveRegToMem(mem, reg, GetType(var));
		operands.ReleaseVariable(var);
		return reg;
	}

	void ExecCompiler::Shelter(const X86Gp & gp)
	{
		if (auto var = operands.FindVariableByGp(gp))
		{
			Comment("Shelter %ls", var->to_string().c_str());
			Reserve(var);
		}
	}

	void ExecCompiler::MoveMemToReg(X86Gp reg, X86Mem base, ValueType valueType)
	{
		switch (valueType) {
		case AnyVal:
			assert(false);
			break;
		case IntegerVal:
		case StringPointerVal:
			CHECK compiler->mov(reg, WithOffsetAndSize(base, OFFSET_SIZE(IntegerValue, value)));
			break;
		case FloatVal:
			assert(false);
			break;
		case BooleanVal:
			CHECK compiler->mov(reg, WithOffsetAndSize(base, OFFSET_SIZE(BooleanValue, value)));
			CHECK compiler->mov(reg, base);
		case NoneVal:
			CHECK compiler->mov(reg, 0);
			break;
		default:
			assert(false);
		}
	}

	void ExecCompiler::MoveRegToMem(X86Mem base, X86Gp reg, ValueType valueType)
	{
		X86Mem typeMem = WithOffsetAndSize(base, OFFSET_SIZE(RuntimeValue, type));
		CHECK compiler->mov(WithOffsetAndSize(base, OFFSET_SIZE(IntegerValue, type)), asmjit::imm_u(valueType));
		switch (valueType) {
		case AnyVal:
			assert(false);
			break;
		case IntegerVal:
		case StringPointerVal:
			CHECK compiler->mov(WithOffsetAndSize(base, OFFSET_SIZE(IntegerValue, value)), reg);
			break;
		case FloatVal:
			assert(false);
			break;
		case BooleanVal:
			CHECK compiler->mov(WithOffsetAndSize(base, OFFSET_SIZE(BooleanValue, value)), reg);
			break;
		case NoneVal:
			CHECK compiler->mov(WithOffsetAndSize(base, OFFSET_SIZE(BooleanValue, value)), 0);
			break;
		default:
			assert(false);
		}
	}

	void ExecCompiler::MemSync(const Variable * var)
	{
		if (operands.InReg(var)) {
			MoveRegToMem(operands.GenerateMem(var), operands.GetReg(var), GetType(var));
		}
	}

	void ExecCompiler::TaskMemMovD(X86Mem dst, X86Mem src, uint32_t sizeDword)
	{
		Comment("MemMovD");

		if (!src.isAbs()) 
		{
			Comment("Used lea src");
			compiler->lea(x86::rsi, src);
		}
		else {
			compiler->mov(x86::rsi, src.getOffset());
		}

		if (!dst.isAbs())
		{
			Comment("Used lea dst");
			compiler->lea(x86::rdi, dst);
		}
		else {
			Comment("Used lea abs");
			compiler->mov(x86::rdi, dst.getOffset());
		}
		/* We should assert DF flag == 0.
		   ref: https://msdn.microsoft.com/en-us/library/7td56tzs.aspx */
		compiler->push(x86::rcx);
		compiler->mov(x86::ecx, sizeDword);
		compiler->asEmitter()->rep().movsd();
		compiler->pop(x86::rcx);
	}

	void ExecCompiler::Ret(Symbol * target, ValueType sourceType)
	{
		Comment("Ret %ls", target->to_string().c_str());
		vector<Operand> op = Locate({
			{ target, true, sourceType },
			{ &Code::BlockResultPtr, true, ObjectPointerVal }
		});
		// TOOD: fix this
		X86Gp valueReg = op[0].as<X86Gp>();
		X86Gp returnValuePtrBaseReg = op[1].as<X86Gp>();
		X86Mem returnValuePtr = asmjit::x86::ptr(returnValuePtrBaseReg,
			offsetof(BlockResult, return_value));
		MoveRegToMem(returnValuePtr, valueReg, sourceType);
	}

	void ExecCompiler::CallNative(Symbol * target, ValueType sourceType)
	{
		NativeFunction * native = dynamic_cast<NativeFunction*>(target);
		ValueType targetType = native->get_ret_type();
		
		Invoke(native->get_func_ptr());
	}

	void ExecCompiler::Push(Symbol * symbol)
	{
		arguments.push_back(Argument{ symbol, false });
	}

	void ExecCompiler::PushParams(const vector<Argument> & arguments)
	{
		uint32_t tempMemSize = 0;
		X86Mem memParamMem = x86::ptr(x86::rsp, ExecStack::BaseCalleeStackSize);
		for (int i = 0; i < arguments.size(); i++)
		{
			const Argument & argument = arguments.at(i);
			Comment("Push param: %ls", argument.symbol->to_string().c_str());
			if (i < ExecOperands::ParameterGps.size())
			{
				X86Gp reg = ExecOperands::ParameterGps.at(i);
				uint32_t incre = PushParamToReg(argument, tempMemSize, reg);
				tempMemSize += incre;
				stackManager->AllocCalleeStack(0);
			}
			else
			{
				uint32_t incre = PushParamToMem(argument, tempMemSize, memParamMem);
				tempMemSize += incre;
				memParamMem.addOffset(sizeof(size_t));
				stackManager->AllocCalleeStack(incre);
			}
		}
	}

	uint32_t ExecCompiler::PushParam(const Argument & argument,
		uint32_t tempMemOffset, std::function<void(X86Mem)> ptrLoader)
	{
		/* https://docs.microsoft.com/zh-cn/cpp/build/parameter-passing
		   Structs/unions of size 8, 16, 32, or 64 bits and
		   __m64 are passed as if they were integers of the same size.  */
		uint32_t symbolSize = sizeof(RuntimeStackSlot);
		// This is temporary, because so far we have `Symbol`s taking up same size
		
		Symbol * symbol = argument.symbol;
		ExecStack::InsertTask taskMakeTemp;
		
		switch (symbolSize)
		{
		case 1:
		case 2:
		case 4:
		case 8:
			// Not implemented
			assert(false);
		default:
			if (auto var = dynamic_cast<const Variable*>(symbol))
			{
				if (!argument.isRef) // If not isRef, duplicate and pass the new one 
				{
					MemSync(var);
					Shelter(x86::rcx);
					Shelter(x86::rdi);
					Shelter(x86::rsi);

					Def(x86::rdi);
					Def(x86::rsi);

					taskMakeTemp = DEF_INSERT_TASK(rbpBased, rspBased) {
						Comment("Alloc Variable Val %ls", symbol->to_string().c_str());
						TaskMemMovD(rbpBased, operands.GenerateMem(var), 4);
						ptrLoader(rbpBased);
						static_assert(sizeof(RuntimeStackSlot) == 16, "Should implement for other sizes of slot. ");
					};
				}
				else // If isRef, pass the pointer
				{
					Comment("MemSync %ls", symbol->to_string().c_str());
					MemSync(var);
					taskMakeTemp = DEF_INSERT_TASK(rbpBased, rspBased) {
						Comment("Alloc Variable Ref %ls", symbol->to_string().c_str());
						ptrLoader(operands.GenerateMem(var));
						static_assert(sizeof(RuntimeStackSlot) == 16, "Should implement for other sizes of slot. ");
					};
				}
			}
			else if (auto literal = dynamic_cast<const Literal*>(symbol))
			{
				X86Mem literalMem = x86::ptr_8_abs((uint64_t)literal->get_value());
				if (!argument.isRef)
				{
					Shelter(x86::rcx);
					Shelter(x86::rdi);
					Shelter(x86::rsi);
					Def(x86::rdi);
					Def(x86::rsi);

					taskMakeTemp = DEF_INSERT_TASK(rbpBased, rspBased) {
						TaskMemMovD(rbpBased, literalMem, 4);
						Comment("Alloc Literal %ls", symbol->to_string().c_str());
						Comment(ToString(literal->get_value()));
						ptrLoader(rbpBased);
						static_assert(sizeof(RuntimeStackSlot) == 16, "Should implement for other sizes of slot. ");
					};
				}
				else 
				{
					MemSync(var);
					taskMakeTemp = DEF_INSERT_TASK(rbpBased, rspBased) {
						Comment("Alloc Literal Ref %ls", symbol->to_string().c_str());
						ptrLoader(literalMem);
						static_assert(sizeof(RuntimeStackSlot) == 16, "Should implement for other sizes of slot. ");
					};
				}
			}
			else 
			{
				assert(false, "Not implemented for this type");
			}
			stackManager->Preinsert(stackManager->Mem(DummyNode(),
				ExecStack::TempMem, tempMemOffset, sizeof(RuntimeStackSlot)), taskMakeTemp);
		}
		return symbolSize;
	}

	uint32_t ExecCompiler::PushParamToReg(const Argument & arg, uint32_t tempMemOffset, const X86Gp & gp)
	{
		return PushParam(arg, tempMemOffset, [=](X86Mem mem) { 
			Comment("PushParamToReg");
			compiler->lea(Def(gp), mem);
		});
	}

	uint32_t ExecCompiler::PushParamToMem(const Argument & arg, uint32_t tempMemOffset, const X86Mem & mem)
	{
		return PushParam(arg, tempMemOffset, [=](X86Mem tempMem) {
			Comment("PushParamToMem");
			compiler->lea(Def(x86::rax), tempMem);
			compiler->mov(mem, x86::rax);
		});
	}

	void ExecCompiler::Invoke(void * funcPtr)
	{
		/* Save ret_reg */
		Shelter(ExecOperands::ret_reg);

		/* Transparent the regs that will change, save them */
		auto savedRegs = operands.GetUsedSaveOnCallGps();

		for (uint32_t i = 0, offset = 0; i < savedRegs.size(); i++, offset += 8)
		{

			const auto task = DEF_INSERT_TASK(rbpBased, rspBased) 
			{
				compiler->mov(rbpBased, savedRegs.at(i));
			};
			stackManager->Preinsert(stackManager->Mem(DummyNode(), 
				ExecStack::SavedRegs, offset, 8), task);
		}

		/* Push params */

		arguments.insert(arguments.begin(), Argument{ Symbol::Acc, true });
		PushParams(arguments);

		/* Call function */
		compiler->asEmitter()->call(reinterpret_cast<uint64_t>(funcPtr));

		/* Transparent the regs that will change, restore them */

		for (uint32_t i = 0, offset = 0; i < savedRegs.size(); i++, offset += 8)
		{

			const auto task = DEF_INSERT_TASK(rbpBased, rspBased) {
				compiler->mov(savedRegs.at(i), rbpBased);
			};
			stackManager->Preinsert(stackManager->Mem(DummyNode(),
				ExecStack::SavedRegs, offset, 8), task);
		}

		arguments.clear();
	}

	ValueType ExecCompiler::GetType(const Symbol * symbol)
	{
		if (auto var = dynamic_cast<const Variable*>(symbol)) {
			if (var->is_const_type()) {
				return var->get_type();
			}
			else {
				return symbolToType->at(var);
			}
		}
		else if (auto literal = dynamic_cast<const Literal*>(symbol)) {
			return literal->get_value()->type;
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

	X86Mem ExecCompiler::WithOffsetAndSize(X86Mem mem, int64_t offset, uint32_t size)
	{
		mem.addOffset(offset);
		mem.setSize(size);
		return mem;
	}

	void ExecCompiler::CompileBinaryImpl(Symbol * target, OpType opType,
		ValueType leftType, Symbol * leftSymbol, ValueType rightType, Symbol * rightSymbol)
	{
		QuadHandlers[opType][leftType][rightType](
			this, target, leftSymbol, rightSymbol
		);
	}

	void ExecCompiler::CompileUnaryImpl(Symbol * target, OpType opType, 
		ValueType type, Symbol * source)
	{
		CompileBinaryImpl(target, opType, type, source, type, source);
	}

	void ExecCompiler::CompileSingleImpl(Symbol * target, BytecodeEnum bytecodeName, ValueType sourceType)
	{
		switch (bytecodeName) {
		case EnumPushParamVariable:
		case EnumPushParamLiteral:
		{
			Push(target);
			break;
		}
		case EnumCallNative:
		{
			CallNative(target, sourceType);
			break;
		}
		case EnumRetAcc:
		{
			Ret(target, sourceType);
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

		epilogInsertingPoint = DummyNode();

		std::vector<X86Gp> usedNonVolatiles = operands.GetUsedNonVolatileGps();
		std::vector<wstring> usedNonVolatilesStrings;
		for (auto & x : usedNonVolatiles)
		{
			usedNonVolatilesStrings.push_back(ExecDebug::to_string(x));
		}
		CLD_DEBUG << LOG_EXPR(CldUtils::Set(usedNonVolatilesStrings)) << std::endl;

		stackManager->Insert(prologInsertingPoint, epilogInsertingPoint,
			usedNonVolatiles);

		compiler->endFunc();
		CHECK compiler->finalize();
		CHECK jitRuntime.add(&func_ptr, &code);
		return Code(func_ptr);
	}
}
