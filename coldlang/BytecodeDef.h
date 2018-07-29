#pragma once
#include "stdafx.h"
#include "BytecodeOperandTypes.h"
#include "NativeFunction.h"

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
	/* call function arg0 with pushed parameters, with return value overwriting Acc */\
	C(CallFunc, AccumulatorUse::Write , OperandType::Symbol*)\
	/* call native function arg0 with pushed parameters, with return value overwriting Acc */\
	C(CallNativeFunc, AccumulatorUse::Write, OperandType::NativeFunction*)\
	/* return accumulator */\
	C(RetAcc, AccumulatorUse::Read)\
	/* push function parameter */\
	C(PushParamVariable, AccumulatorUse::None, OperandType::Variable*)\
	C(PushParamLiteral, AccumulatorUse::None, OperandType::Literal*)\
	C(PushParamAcc, AccumulatorUse::Read)\
	/* jump to arg0 */\
	C(Jump, AccumulatorUse::None, OperandType::Label*)\
	/* jump to arg0 if Acc is true */\
	C(JumpOnTrue, AccumulatorUse::Read, OperandType::Label*)\
	/* jump to arg1 if Acc is false */\
	C(JumpOnFalse, AccumulatorUse::Read, OperandType::Label*)\
	/* logical or with Acc */\
	C(Or, AccumulatorUse::ReadWrite, OperandType::Symbol*)\
	C(OrImm64, AccumulatorUse::ReadWrite, OperandType::Imm64*)\
	/* logical and with Acc */\
	C(And, AccumulatorUse::ReadWrite, OperandType::Symbol*)\
	C(AndImm64, AccumulatorUse::ReadWrite, OperandType::Imm64*)\
	/* logical equal with Acc */\
	C(Equal, AccumulatorUse::ReadWrite, OperandType::Symbol*)\
	C(EqualImm64, AccumulatorUse::ReadWrite, OperandType::Imm64*)\
	/* logical not-equal with Acc */\
	C(NotEqual, AccumulatorUse::ReadWrite, OperandType::Symbol*)\
	C(NotEqualImm64, AccumulatorUse::ReadWrite, OperandType::Imm64*)\
	/* signed GE with Acc */\
	C(GreaterEqual, AccumulatorUse::ReadWrite, OperandType::Symbol*)\
	C(GreaterEqualImm64, AccumulatorUse::ReadWrite, OperandType::Imm64*)\
	/* signed LE with Acc */\
	C(LessEqual, AccumulatorUse::ReadWrite, OperandType::Symbol*)\
	C(LessEqualImm64, AccumulatorUse::ReadWrite, OperandType::Imm64*)\
	/* signed GT with Acc */\
	C(GreaterThan, AccumulatorUse::ReadWrite, OperandType::Symbol*)\
	C(GreaterThanImm64, AccumulatorUse::ReadWrite, OperandType::Imm64*)\
	/* signed LT with Acc */\
	C(LessThan, AccumulatorUse::ReadWrite, OperandType::Symbol*)\
	C(LessThanImm64, AccumulatorUse::ReadWrite, OperandType::Imm64*)\
	/* add with Acc */\
	C(Add, AccumulatorUse::ReadWrite, OperandType::Symbol*)\
	C(AddImm64, AccumulatorUse::ReadWrite, OperandType::Imm64*)\
	/* subtract with Acc */\
	C(Sub, AccumulatorUse::ReadWrite, OperandType::Symbol*)\
	C(SubImm64, AccumulatorUse::ReadWrite, OperandType::Imm64*)\
	/* multiply with Acc */\
	C(Mul, AccumulatorUse::ReadWrite, OperandType::Symbol*)\
	C(MulImm64, AccumulatorUse::ReadWrite, OperandType::Imm64*)\
	/* binary multiply to Acc */\
	C(MulTwo, AccumulatorUse::ReadWrite, OperandType::Symbol*, OperandType::Symbol*)\
	/* divide with Acc */\
	C(Div, AccumulatorUse::ReadWrite, OperandType::Symbol*)\
	C(DivImm64, AccumulatorUse::ReadWrite, OperandType::Imm64*)\
	/* binary divide to Acc */\
	C(DivTwo, AccumulatorUse::ReadWrite, OperandType::Symbol*, OperandType::Symbol*)\
	/* mod with Acc */\
	C(Mod, AccumulatorUse::ReadWrite, OperandType::Symbol*)\
	C(ModImm64, AccumulatorUse::ReadWrite, OperandType::Imm64*)\
	/* binary mod to Acc */\
	C(ModTwo, AccumulatorUse::ReadWrite, OperandType::Symbol*, OperandType::Symbol*)\
	/* incremenmt arg0 */\
	C(Inc, AccumulatorUse::ReadWrite, OperandType::Symbol*)\
	/* decrement arg0 */\
	C(Decre, AccumulatorUse::ReadWrite, OperandType::Symbol*)\
	/* negate Acc, that is to multiply with -1 */\
	C(Neg, AccumulatorUse::ReadWrite)\
	/* load variable to accumulator */\
	C(LoadVariableToAcc, AccumulatorUse::Write, OperandType::Variable*)\
	/* load literal to accumulator */\
	C(LoadLiteralToAcc, AccumulatorUse::Write, OperandType::Literal*)\
	/* visit attribute arg1 of arg0 and load it to accumulator */\
	C(LoadAttributeToAcc, AccumulatorUse::Write, OperandType::Symbol*, OperandType::Index*)\
	/* store Acc to arg0 */\
	C(StoreAcc, AccumulatorUse::Read, OperandType::Symbol*)

}