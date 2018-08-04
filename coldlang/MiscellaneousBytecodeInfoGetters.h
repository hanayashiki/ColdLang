#pragma once
#include "BytecodeClass.h"
#include "BytecodeBase.h"

namespace IR
{
	namespace BytecodeClass
	{
		static UnaryInfo get_unary_info_StoreAcc(unsigned char buf[])
		{
			return {
				OpMove, retrieve_arg<OperandType::Variable*>(&buf[1])
			};
		}
	}
}