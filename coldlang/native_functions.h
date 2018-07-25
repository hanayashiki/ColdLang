#pragma once
#include "stdafx.h"
#include "RuntimeObject.h"

namespace Runtime
{
	void native_puts(PointerValue pointer);

#define NATIVE_FUNCTION_DEF(C, container)	\
	C(container, native_puts)	\

#define ADD_NATIVE_FUNCTION(container, func_name) \
	static_assert(std::is_same<std::remove_reference<decltype(container)>::type::value_type,\
					std::tuple<const wchar_t *, void *>>::value); \
	container.push_back(std::make_tuple(L###func_name, reinterpret_cast<void*>(&func_name)));

#define ADD_NATIVE_FUNCTIONS(container)	\
	NATIVE_FUNCTION_DEF(ADD_NATIVE_FUNCTION, container)

	template<typename ContainerType>
	static void add_native_functions(ContainerType & container)
	{
		ADD_NATIVE_FUNCTIONS(container);
	}
}