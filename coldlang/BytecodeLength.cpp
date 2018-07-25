#include "stdafx.h"
#include "BytecodeLength.h"

namespace IR
{
	namespace BytecodeClass {
#define FILL_ARG_LENGTH(bytecode_name, acc_use, ...) \
			BytecodeClass::get_arg_length<__VA_ARGS__>::value + 1,
		const size_t bytecode_to_length[] =
		{
			BYTECODE_LIST(FILL_ARG_LENGTH)
		}; 

		size_t get_bytecode_length(unsigned char id) {
			assert(id >= 0 && id <= 255);
			return bytecode_to_length[id];
		}
#undef FILL_ARG_LENGTH
	}

}