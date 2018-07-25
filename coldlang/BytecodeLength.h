#pragma once
#include "stdafx.h"
#include "NativeFunction.h"


namespace IR
{
	namespace BytecodeClass
	{

		template<typename T>
		struct get_one_length
		{
			const static size_t value = sizeof(T);
		};

		//template<OperandType::NativeFunction*>
		//struct get_one_length
		//{
		//	static constexpr size_t value = sizeof(OperandType::NativeFunction*);
		//};
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