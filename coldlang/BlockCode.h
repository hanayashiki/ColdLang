#pragma once
#include "BasicBlock.h"
#include "BytecodeTyper.h"

namespace CldRuntime
{
	class BlockCode
	{
	public:
		using ptr = std::shared_ptr<BlockCode>;

		BlockCode();
		explicit BlockCode(size_t begin, size_t end);

		size_t get_begin();
		size_t get_end();
	private:
		size_t begin = 0;
		size_t end = 0;
		IR::SymbolToType type_reqs;
		vector<BlockCode::ptr> next_block_codes;
	};
}
