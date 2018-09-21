#pragma once
#include <cstdint>
#include "log.h"

#define TYPE_SIGNATURE(sig) enum { Signature = sig };

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
		TYPE_SIGNATURE(AnyVal)
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
		TYPE_SIGNATURE(IntegerVal)
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
		TYPE_SIGNATURE(FloatVal)
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
		TYPE_SIGNATURE(BooleanVal)
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
		TYPE_SIGNATURE(NoneVal)
		NoneValue() : RuntimeValue(NoneVal)
		{
			CLD_DEBUG << LOG_EXPR(this) << std::endl;
			CLD_DEBUG << "NoneValue Initializer called" << std::endl;
		}
	};

	struct RuntimeObject;
	struct PointerValue : RuntimeValue
	{
		TYPE_SIGNATURE(StringPointerVal)
		RuntimeObject * value;
		PointerValue(RuntimeObject * value)
			: RuntimeValue(StringPointerVal), value(value)
		{
		}
	};
}

#include "RuntimeValueUtils.h"