#pragma once
#include "stdafx.h"
#include "BytecodeWriter.h"

namespace IR
{
	class BytecodeDumper : public BytecodeWriter
	{
	private:
		BytecodeWriter * next_writer = nullptr;
	public:
		void pass_to(BytecodeWriter * w)
		{
			next_writer = w;
		}

		void emit(BytecodeClass::BytecodeBase & bytecode) override
		{
			// todo
			wcout << bytecode.to_string() << endl;
			if (next_writer)
			{
				next_writer->emit(bytecode);
			}
		}
		void bind(OperandType::Label & label) override
		{
			wcout << label.to_string() + L":" << endl;
		}
	};
}
