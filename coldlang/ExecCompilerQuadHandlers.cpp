#include "stdafx.h"
#include "ExecCompiler.h"
#include "log.h"
#include "ExecDebug.h"
#include "BytecodeClass.h"

using namespace asmjit;
using CldRuntime::BlockEnv;

#define DEF_QUAD_HANDLER_HEAD(_this, target, left, right) \
	QuadHandler([](ExecCompiler* ##_this##, Symbol* ##target##, Symbol* ##left##, Symbol* ##right##)

#define INTEGER_QUAD_EMIT(_this, exprOperands, opType) \
	_this->integerEmitter->Emit(exprOperands[0], opType, \
		exprOperands[1], exprOperands[2]);

#define LOAD_QUAD_OPERANDS_AND_EMIT(_this, target, left, right, opType, valueType) \
	vector<asmjit::Operand> exprOperands = \
		_this->LocateQuad(target, valueType, left, right); \
	_this->Comment("%ls %s %ls %ls", IR::to_string(target).c_str(), \
		IR::BytecodeClass::OpTypeName[opType], \
		IR::to_string(left).c_str(), \
		IR::to_string(right).c_str()); \
	INTEGER_QUAD_EMIT(_this, exprOperands, opType); 

#define DEF_QUAD_HANDLER(opType, valueType) \
	DEF_QUAD_HANDLER_HEAD(_this, target, left, right) { LOAD_QUAD_OPERANDS_AND_EMIT(_this, target, left, right, opType, valueType) return true; } );


namespace Compile {

	vector<asmjit::Operand> ExecCompiler::LocateQuad(Symbol * target, ValueType type, Symbol * left, Symbol * right)
	{
		return std::move(
			Locate({ { target, false, type }, { left, true, type }, { right, true, type } })
		);
	}

	void ExecCompiler::InitQuadHandlers()
	{
		QuadHandlers[OpAdd][IntegerVal][IntegerVal]		= DEF_QUAD_HANDLER(OpAdd,	IntegerVal);
		QuadHandlers[OpSub][IntegerVal][IntegerVal]		= DEF_QUAD_HANDLER(OpSub,	IntegerVal);
		QuadHandlers[OpMove][IntegerVal][IntegerVal]	= DEF_QUAD_HANDLER(OpMove,	IntegerVal);
	}

	
}