#pragma once
#include "stdafx.h"
#include "NativeFunction.h"
#include "BytecodeArgumentDump.h"

namespace IR
{
	namespace BytecodeClass
	{
		template<typename... Args>
		struct get_arg_length;

		template<typename T, typename... Args>
		struct get_arg_length<T, Args...>
		{
			static constexpr size_t value = get_one_length<T>::value + get_arg_length<Args...>::value;
		};

		template<typename T>
		struct get_arg_length<T>
		{
			static constexpr size_t value = get_one_length<T>::value;
		};

		template<typename... Args>
		struct get_arg_length
		{
			static constexpr size_t value = 0;
		};

		size_t get_bytecode_length(unsigned char id);

	}
}