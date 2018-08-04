#pragma once

namespace IR
{
	namespace BytecodeClass
	{
		class BytecodeBase
		{
		public:
			virtual size_t dump_byte(unsigned char []) = 0;
			virtual wstring get_name() = 0;
			virtual wstring to_string() = 0;
			virtual ~BytecodeBase() {}
		};

		enum OpType;

		struct UnaryInfo
		{
			OpType op;
			OperandType::Symbol* symbol;
		};

		class BytecodeUnary : public BytecodeBase
		{
		};
	}
}