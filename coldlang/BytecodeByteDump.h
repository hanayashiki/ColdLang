#pragma once
#include "stdafx.h"
#include "BytecodeArgumentDump.h"

namespace IR
{
	namespace BytecodeClass
	{
		enum BytecodeEnum;

		static size_t dump_args(unsigned char* buf)
		{
			return 0;
		}

		template<typename T, typename ...Args>
		static size_t dump_args(unsigned char* buf, T arg, Args... code_args)
		{
			unsigned char * old_buf = buf;
			buf += dump_one_arg(buf, arg);
			buf += dump_args(buf, code_args...);
			return buf - old_buf;
		}

		template<typename ...Args>
		size_t params_to_byte(unsigned char* buf, BytecodeEnum code_enum, Args... code_args)
		{
			unsigned char * start = buf;
			char truncate_code_enum = code_enum & 0xff;
			buf[0] = truncate_code_enum;
			buf++;
			size_t len = dump_args(buf, code_args...) + 1;
			//wcout << mem_to_string(start, len).c_str() << endl;
			return len;
		}
	}
}
