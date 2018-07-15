#pragma once

namespace IR
{
	namespace BytecodeClass
	{
		class BytecodeBase
		{
		public:
			virtual void dump_byte(char []) = 0;
			virtual const char * get_name() = 0;
			virtual wstring to_string() = 0;
			virtual ~BytecodeBase() {}
		};
	}
}