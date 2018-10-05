#pragma once
#include "stdafx.h"
#include "BytecodeByteDump.h"

#include "utils.h"

namespace IR
{
	class BytecodeVectorWriter : public BytecodeWriter
	{
		BytecodeWriter * next_writer_;
		vector<uint8_t> * vec_;
	public:
		BytecodeVectorWriter(vector<uint8_t>* vec)
			:vec_(vec)
		{
			
		}

		virtual void pass_to(BytecodeWriter * w) override
		{
			next_writer_ = w;
		}

		virtual void emit(BytecodeClass::BytecodeBase & bytecode) override
		{
			unsigned char buf[100];
			size_t len = bytecode.dump_byte(buf);
			assert(len < sizeof(buf));
			for (int i = 0; i < len; i++)
			{
				vec_->push_back(buf[i]);
			}
			if (len > 0)
			{
				wcout << "[" << CldUtils::MemToString(buf, len).c_str() << "]" << endl;
			}
			if (next_writer_)
			{
				next_writer_->emit(bytecode);
			}
		}

		void bind(OperandType::Label & label) override
		{
			BytecodeClass::Label label_place_holder(&label);
			emit(label_place_holder);
			if (next_writer_)
			{
				next_writer_->bind(label);
			}
		}
	};
}
