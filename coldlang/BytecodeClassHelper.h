#pragma once
#include "BytecodeClass.h"

namespace IR
{
	namespace BytecodeClass
	{
		static bool is_conditional_jump(const unsigned char buf[])
		{
			return buf[0] == EnumJumpOnTrue || buf[0] == EnumJumpOnFalse;
		}

		static bool is_unconditional_jump(const unsigned char buf[])
		{
			return buf[0] == EnumJump;
		}

		static bool is_jump(const unsigned char buf[])
		{
			return is_unconditional_jump(buf) || is_conditional_jump(buf);
		}

		static bool is_ret(const unsigned char buf[])
		{
			return buf[0] == EnumRetAcc;
		}

		static bool is_calculation(const unsigned char buf[])
		{
			return !is_jump(buf) && !is_ret(buf);
		}

		static bool is_label(const unsigned char buf[])
		{
			return buf[0] == EnumLabel;
		}

	}
}