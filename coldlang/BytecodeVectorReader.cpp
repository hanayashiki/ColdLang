#include "stdafx.h"
#include "BytecodeVectorReader.h"
#include "BytecodeLength.h"

namespace IR {

	using namespace BytecodeClass;

	BytecodeVectorReader::BytecodeVectorReader(vector<uint8_t>* bytecode_vector)
		: bytecode_vector_(bytecode_vector), offset(0)
	{
		
	}

	void BytecodeVectorReader::read_byte(unsigned char buf[], size_t* count)
	{
		if (offset < bytecode_vector_->size()) {
			size_t len = get_bytecode_length(bytecode_vector_->at(offset));
			*count = len;
			memcpy(buf, bytecode_vector_->data() + offset, len);
			offset += len;
		} else
		{
			*count = 0;
		}
	}

}
