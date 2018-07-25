#pragma once
#include "stdafx.h"
#include "BytecodeReader.h"

namespace IR
{
	class BytecodeVectorReader : public BytecodeReader
	{
	private:
		size_t offset;
		vector<uint8_t> * bytecode_vector_;
	public:
		BytecodeVectorReader(vector<uint8_t> * bytecode_vector);
		void read_byte(unsigned char buf[], size_t* count) override;
	};
}