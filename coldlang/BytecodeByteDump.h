#pragma once
#include "stdafx.h"
#include "BytecodeEnum.h"
#include "BytecodeArgumentDump.h"
#include "BytecodeClass.h"

namespace IR
{
	namespace BytecodeClass
	{
		static string mem_to_string(unsigned char * start, size_t len)
		{
			string str;
			for (size_t i = 0; i < len; i++)
			{
				char wbuf[32];
				sprintf_s<sizeof(wbuf)>(wbuf, "%02x ", start[i] & 0xff);
				str += wbuf;
				if ((i + 1) % 4 == 0)
				{
					str += "   ";
				}
				else if ((i + 1) % 16 == 0)
				{
					str += "\n";
				}
			}
			return str;
		}

		static size_t dump_args(unsigned char* buf)
		{
			return 0;
		}

		template<typename T, typename ...Args>
		static size_t dump_args(unsigned char* buf, T arg, Args... code_args)
		{
			char * old_buf = buf;
			buf += dump_one_arg(buf, arg);
			buf += dump_args(buf, code_args...);
			return buf - old_buf;
		}

		template<typename ...Args>
		size_t params_to_byte(unsigned char* buf, BytecodeEnum code_enum, Args... code_args)
		{
			char * start = buf;
			char truncate_code_enum = code_enum & 0xff;
			buf[0] = truncate_code_enum;
			buf++;
			size_t len = dump_args(buf, code_args...) + 1;
			//wcout << mem_to_string(start, len).c_str() << endl;
			return len;
		}
	}
}
