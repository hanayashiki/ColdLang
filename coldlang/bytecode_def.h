#pragma once
#include "stdafx.h"
#include "bytecode_operand_types.h"

namespace IR
{
	namespace AccumulatorUse
	{
		enum Type {
			None = 0,
			Read = 1 << 0,
			Write = 1 << 1,
			ReadWrite = Read | Write
		};
	}
}

namespace IR
{
#define BYTECODE_LIST(C)\
	/* visit attribute arg1 of arg0 and load it to accumulator */\
	C(LoadAttributeToAcc, AccumulatorUse::Write, OperandType::Symbol, OperandType::Index)\
	/* call function arg0 with pushed parameters, with return value overwriting Acc */\
	C(CallFunc, AccumulatorUse::Write, OperandType::Index)\
	/* push function parameter */\
	C(PushParamImm64, AccumulatorUse::None, OperandType::Imm64)\
	C(PushParamSymbol, AccumulatorUse::None, OperandType::Symbol)\
	C(PushParamAcc, AccumulatorUse::Read)\
	/* jump to arg0 if Acc is true */\
	C(JumpOnTrue, AccumulatorUse::Read, OperandType::Label)\
	/* jump to arg1 if Acc is false */\
	C(JumpOnFalse, AccumulatorUse::Write, OperandType::Label)\
	/* logical or with Acc */\
	C(Or, AccumulatorUse::ReadWrite, OperandType::Symbol)\
	C(OrImm64, AccumulatorUse::ReadWrite, OperandType::Imm64)\
	/* logical and with Acc */\
	C(And, AccumulatorUse::ReadWrite, OperandType::Symbol)\
	C(AndImm64, AccumulatorUse::ReadWrite, OperandType::Imm64)\
	/* logical equal with Acc */\
	C(Equal, AccumulatorUse::ReadWrite, OperandType::Symbol)\
	C(EqualImm64, AccumulatorUse::ReadWrite, OperandType::Imm64)\
	/* logical not-equal with Acc */\
	C(NotEqual, AccumulatorUse::ReadWrite, OperandType::Symbol)\
	C(NotEqualImm64, AccumulatorUse::ReadWrite, OperandType::Imm64)\
	/* signed GE with Acc */\
	C(GreaterEqual, AccumulatorUse::ReadWrite, OperandType::Symbol)\
	C(GreaterEqualImm64, AccumulatorUse::ReadWrite, OperandType::Imm64)\
	/* signed LE with Acc */\
	C(LessEqual, AccumulatorUse::ReadWrite, OperandType::Symbol)\
	C(LessEqualImm64, AccumulatorUse::ReadWrite, OperandType::Imm64)\
	/* signed GT with Acc */\
	C(GreaterThan, AccumulatorUse::ReadWrite, OperandType::Symbol)\
	C(GreaterThanImm64, AccumulatorUse::ReadWrite, OperandType::Imm64)\
	/* signed LT with Acc */\
	C(LessThan, AccumulatorUse::ReadWrite, OperandType::Symbol)\
	C(LessThanImm64, AccumulatorUse::ReadWrite, OperandType::Imm64)\
	/* add with Acc */\
	C(Add, AccumulatorUse::ReadWrite, OperandType::Symbol)\
	C(AddImm64, AccumulatorUse::ReadWrite, OperandType::Imm64)\
	/* subtract with Acc */\
	C(Sub, AccumulatorUse::ReadWrite, OperandType::Symbol)\
	C(SubImm64, AccumulatorUse::ReadWrite, OperandType::Imm64)\
	/* multiply with Acc */\
	C(Mul, AccumulatorUse::ReadWrite, OperandType::Symbol)\
	C(MulImm64, AccumulatorUse::ReadWrite, OperandType::Imm64)\
	/* divide with Acc */\
	C(Div, AccumulatorUse::ReadWrite, OperandType::Symbol)\
	C(DivImm64, AccumulatorUse::ReadWrite, OperandType::Imm64)\
	/* mod with Acc */\
	C(Mod, AccumulatorUse::ReadWrite, OperandType::Symbol)\
	C(ModImm64, AccumulatorUse::ReadWrite, OperandType::Imm64)\
	/* incremenmt Acc */\
	C(Inc, AccumulatorUse::ReadWrite)\
	/* decrement Acc */\
	C(Decre, AccumulatorUse::ReadWrite)\
	/* negate Acc, that is to multiply with -1 */\
	C(Neg, AccumulatorUse::ReadWrite)\

}