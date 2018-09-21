#include "stdafx.h"
#include "NativeFunctionsDefs.h"
#include "RuntimeValue.h"
#include "RuntimeObject.h"

#include "log.h"

#define NATIVE_FUNC(function_name) NativeFunctionsDefs::make_native(L###function_name, &function_name)

namespace CldRuntime
{
	NoneValue native_puts(PointerValue pointer)
	{
		assert(pointer.type == StringPointerVal);
		RuntimeObject * runtime_obj = pointer.value;
		assert(runtime_obj->type == StringObj);
		StringObject * string_object = reinterpret_cast<StringObject*>(runtime_obj);
		wcout << string_object->content;
		return NoneValue();
	}

	IntegerValue native_test_7_args(
		IntegerValue val1,
		IntegerValue val2,
		IntegerValue val3,
		IntegerValue val4,
		IntegerValue val5,
		IntegerValue val6,
		IntegerValue val7)
	{
		return IntegerValue(val1.value + val2.value + val3.value + val4.value +
			val5.value + val6.value + val7.value);
	}

	IntegerValue native_test_3_args(
		IntegerValue val1,
		IntegerValue val2,
		IntegerValue val3)
	{	
		CLD_DEBUG << LOG_EXPR(val1.value) << ", "
			<< LOG_EXPR(val2.value) << ", "
			<< LOG_EXPR(val3.value) << std::endl;
		return IntegerValue(val1.value + val2.value + val3.value);
	}

	std::vector<NativeFunctionsDefs::NativeFunction> 
	NativeFunctionsDefs::native_functions = 
	{
		NATIVE_FUNC(native_puts),
		NATIVE_FUNC(native_test_7_args),
		NATIVE_FUNC(native_test_3_args),
	};

	std::vector<NativeFunctionsDefs::NativeFunction> &
	NativeFunctionsDefs::get_native_functions()
	{
		return native_functions;
	}

}