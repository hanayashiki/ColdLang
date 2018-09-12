#pragma once
#include <cstdint>

namespace CldRuntime
{
	enum ValueType
	{
		AnyVal,
		IntegerVal,
		FloatVal,
		BooleanVal,
		NoneVal,
		StringPointerVal,
		FunctionPointerVal,
		ObjectPointerVal,
		ErrorVal
	};

	static const ValueType ValueVecEnd = ErrorVal;

	static const char * ValueTypeName[] = {
		"any",
		"int",
		"float",
		"bool",
		"none",
		"string_ptr",
		"func_ptr",
		"obj_ptr",
		"err_ptr"
	};

	static const size_t ValueTypeCount = sizeof(ValueTypeName) / sizeof(ValueTypeName[0]);

	enum ObjectType
	{
		StringObj = 1,
		FunctionObj
	};

	struct RuntimeValue
	{
		ValueType type;
		RuntimeValue() : type(NoneVal)
		{
		}
		RuntimeValue(ValueType type) : type(type)
		{
		}
	};

	struct IntegerValue : RuntimeValue
	{
		int64_t value = 0;
		IntegerValue()
		{
		}
		IntegerValue(int64_t value) : value(value), RuntimeValue(IntegerVal)
		{
		}
	};

	struct FloatValue : RuntimeValue
	{
		double value = .0;
		FloatValue()
		{
		}
		FloatValue(double value) : value(value), RuntimeValue(FloatVal)
		{
		}
	};

	struct BooleanValue : RuntimeValue
	{
		bool value = false;
		BooleanValue()
		{
		}
		BooleanValue(bool value) : value(value), RuntimeValue(BooleanVal)
		{
		}
	};

	struct NoneValue : RuntimeValue
	{
		NoneValue() : RuntimeValue(NoneVal)
		{
		}
	};

	struct RuntimeObject;
	struct PointerValue : RuntimeValue
	{
		// TODO : fix this
		RuntimeObject * value;
	};
}