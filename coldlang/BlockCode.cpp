#include "stdafx.h"
#include "BlockCode.h"

namespace CldRuntime {
	BlockCode::BlockCode(vector<IR::BasicBlock>::size_type b)
		: block_id(b)
	{
	}

	BlockCode::BlockCode(const BlockCode & b) noexcept
		: block_id(b.block_id), type_to_code(b.type_to_code)
	{
	}

	BlockCode::BlockCode(BlockCode && b) noexcept
		: block_id(b.block_id), type_to_code(std::move(b.type_to_code))
	{

	}

	BlockCode & BlockCode::operator=(const BlockCode & b) noexcept
	{
		block_id = b.block_id;
		type_to_code = b.type_to_code;
		return *this;
	}

	BlockCode & BlockCode::operator=(BlockCode && b) noexcept
	{
		block_id = b.block_id;
		type_to_code = b.type_to_code;
		return *this;
	}


}
