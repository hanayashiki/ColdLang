#pragma once
#include "stdafx.h"
#include "BytecodeWriter.h"

namespace IR
{
	class BytecodeDumper : public BytecodeWriter
	{
	private:
		BytecodeWriter * next_writer_ = nullptr;
	public:
		void pass_to(BytecodeWriter * w)
		{
			next_writer_ = w;
		}

		void emit(BytecodeClass::BytecodeBase & bytecode) override
		{
			// todo
			wcout << bytecode.to_string() << endl;
			if (next_writer_)
			{
				next_writer_->emit(bytecode);
			}
		}
		void bind(OperandType::Label & label) override
		{
			wcout << label.to_string() + L":" << endl;
			if (next_writer_)
			{
				next_writer_->bind(label);
			}
		}
	};
}
