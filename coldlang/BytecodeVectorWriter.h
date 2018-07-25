#pragma once
#include "stdafx.h"
#include "BytecodeByteDump.h"

namespace IR
{
	class BytecodeVectorWriter : public BytecodeWriter
	{
		BytecodeWriter * next_writer;
		vector<uint8_t>* vec_;
	public:
		BytecodeVectorWriter(vector<uint8_t>* vec)
			:vec_(vec)
		{
			
		}

		virtual void pass_to(BytecodeWriter * w)
		{
			next_writer = w;
		}

		virtual void emit(BytecodeClass::BytecodeBase & bytecode) override
		{
			char buf[100];
			size_t len = bytecode.dump_byte(buf);
			assert(len < sizeof(buf));
			for (int i = 0; i < len; i++)
			{
				vec_->push_back(buf[i]);
			}
			if (len > 0)
			{
				// wcout << "[" << BytecodeClass::ByteDump::mem_to_string(buf, len).c_str() << "]" << endl;
			}
			if (next_writer)
			{
				next_writer->emit(bytecode);
			}
		}
		void bind(OperandType::Label & label) override
		{
			//wcout << label.to_string() + L":" << endl;
		}
	};
}
