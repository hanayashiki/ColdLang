#pragma once
#include "RuntimeObject.h"

namespace CldRuntime
{
	typedef ValueType* ValueTypeVec;

	static bool equal(const ValueTypeVec & v1, const ValueTypeVec & v2) 
	{
		auto _v1 = v1;
		auto _v2 = v2;
		while (*_v1 != ValueVecEnd || *_v2 != ValueVecEnd)
		{
			if (*_v1 != *_v2)
			{
				return false;
			}
			_v1++;
			_v2++;
		}
		return true;
	}

	static size_t hash(const ValueTypeVec & v)
	{
		ValueTypeVec cur = v;
		const static size_t base = 19260817;
		size_t ans = 0;
		while (*cur != ValueVecEnd)
		{
			ans = ans * base + *cur;
			cur++;
		}
		return ans;
	}

	static size_t len(const ValueTypeVec & v)
	{
		ValueTypeVec cur = v;
		size_t len = 0;
		while (*cur != ValueVecEnd)
		{
			len++;
			cur++;
		}
		return len;
	}

	static size_t size(const ValueTypeVec & v)
	{
		return len(v) + 1;
	}

};