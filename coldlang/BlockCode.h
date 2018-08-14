#pragma once
#include "TypeToCode.h"
#include "BasicBlock.h"

namespace CldRuntime
{
	class BlockCode
	{
	public:
		vector<IR::BasicBlock>::size_type block_id;
		TypeToCode type_to_code;

		explicit BlockCode(vector<IR::BasicBlock>::size_type);

		BlockCode(const BlockCode &) noexcept;
		BlockCode(BlockCode &&) noexcept;
		BlockCode & operator=(const BlockCode &) noexcept;
		BlockCode & operator=(BlockCode &&) noexcept;
	};
}
