#include "stdafx.h"
#include "TypeToCode.h"

namespace CldRuntime {
	TypeToCode::TypeToCode(const TypeToCode & t) noexcept
		: map(t.map)
	{
	}
	TypeToCode::TypeToCode(TypeToCode && t) noexcept
		: map(t.map)
	{
	}
	TypeToCode & TypeToCode::operator=(const TypeToCode & t) noexcept
	{
		map = t.map;
		return *this;
	}
	TypeToCode & TypeToCode::operator=(TypeToCode && t) noexcept
	{
		map = std::move(t.map);
		return *this;
	}
}
