#pragma once

namespace IR 
{
	class BytecodeReader
	{
	public:
		virtual void read_byte(unsigned char buf[], size_t * count) = 0;

	};
}