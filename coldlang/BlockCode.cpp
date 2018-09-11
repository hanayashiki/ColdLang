#include "stdafx.h"
#include "BlockCode.h"

namespace CldRuntime {
	BlockCode::BlockCode()
	{
	}

	BlockCode::BlockCode(size_t begin, size_t end)
		: begin(begin), end(end)
	{
	}

	size_t BlockCode::get_begin()
	{
		return begin;
	}

	size_t BlockCode::get_end()
	{
		return end;
	}


}
