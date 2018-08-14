#pragma once
#include <unordered_map>
#include "Code.h"
#include "RuntimeObject.h"
#include "ValueTypeVec.h"

namespace CldRuntime
{
	class TypeToCode
	{
	private:
		struct ValueTypeHash
		{
			size_t operator()(const ValueTypeVec& v) const
			{
				return hash(v);
			}
		};
		struct ValueTypeEqual
		{
			bool operator()(const ValueTypeVec &v1, const ValueTypeVec &v2) const
			{
				return equal(v1, v2);
			}
		};
		typedef unordered_map<ValueTypeVec, Compile::Code, ValueTypeHash, ValueTypeEqual> Map;
	public:
		Map map;

		TypeToCode() = default;
		TypeToCode(const TypeToCode &) noexcept;
		TypeToCode(TypeToCode &&) noexcept;
		TypeToCode & operator=(const TypeToCode &) noexcept;
		TypeToCode & operator=(TypeToCode &&) noexcept;
	};
}
