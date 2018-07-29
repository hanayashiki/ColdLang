#pragma once
#include "stdafx.h"
#include "RuntimeObject.h"

#define COMBINE(val1, val2) (((val1) << 16) + (val2))

namespace Runtime
{
	class TypeCalc
	{
	public:
		static ValueType type_calc_add(ValueType val1, ValueType val2, bool swapped = false)
		{
			switch(COMBINE(val1, val2))
			{
			case COMBINE(AnyVal, AnyVal):
			case COMBINE(AnyVal, IntegerVal):
			case COMBINE(AnyVal, FloatVal):
			case COMBINE(AnyVal, StringPointerVal):
			case COMBINE(AnyVal, FunctionPointerVal):
			case COMBINE(AnyVal, ObjectPointerVal):
				return AnyVal;
			case COMBINE(IntegerVal, IntegerVal):
				return IntegerVal;
			case COMBINE(IntegerVal, FloatVal):
				return FloatVal;
			case COMBINE(IntegerVal, StringPointerVal):
				return StringPointerVal;
			case COMBINE(FloatVal, FloatVal):
				return FloatVal;
			case COMBINE(FloatVal, StringPointerVal):
				return StringPointerVal;
			case COMBINE(FunctionPointerVal, StringPointerVal):
				return StringPointerVal;
			case COMBINE(ObjectPointerVal, IntegerVal):
			case COMBINE(ObjectPointerVal, FloatVal):
			case COMBINE(ObjectPointerVal, StringPointerVal):
			case COMBINE(ObjectPointerVal, FunctionPointerVal):
			case COMBINE(ObjectPointerVal, ObjectPointerVal):
				return AnyVal;
			default:
				if (!swapped) {
					return type_calc_add(val2, val1, true);
				}
				else {
					return ErrorVal;
				}
			}
		}

		static ValueType type_calc_mult_div(ValueType val1, ValueType val2, bool swapped = false)
		{
			switch(COMBINE(val1, val2))
			{
			case COMBINE(ObjectPointerVal, IntegerVal):
			case COMBINE(ObjectPointerVal, FloatVal):
			case COMBINE(ObjectPointerVal, StringPointerVal):
			case COMBINE(ObjectPointerVal, FunctionPointerVal):
			case COMBINE(ObjectPointerVal, ObjectPointerVal):
				return AnyVal;
			case COMBINE(AnyVal, IntegerVal):
			case COMBINE(AnyVal, FloatVal):
				return AnyVal;
			case COMBINE(IntegerVal, IntegerVal):
				return IntegerVal;
			case COMBINE(FloatVal, IntegerVal):
				return FloatVal;
			case COMBINE(FloatVal, FloatVal):
				return FloatVal;
			default:
				if (!swapped)
				{
					return type_calc_mult_div(val2, val1, true);
				} else
				{
					return ErrorVal;
				}
			}
		}

	};
}

#undef COMBINE