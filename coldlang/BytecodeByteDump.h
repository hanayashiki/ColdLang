#pragma once
#include "stdafx.h"
#include "BytecodeEnum.h"

namespace IR
{
	namespace BytecodeClass
	{
		namespace ByteDump 
		{
			static string mem_to_string(char * start, size_t len)
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

			static size_t dump_one_arg(char* buf, NativeFunction* native_func)
			{
				memcpy(buf, &native_func, sizeof(native_func));
				return sizeof(native_func);
			}

			static size_t dump_one_arg(char* buf, Literal* literal)
			{
				memcpy(buf, &literal, sizeof(literal));
				return sizeof(literal);
			}

			static size_t dump_one_arg(char* buf, Variable* variable)
			{
				memcpy(buf, &variable, sizeof(variable));
				return sizeof(variable);
			}

			static size_t dump_args(char* buf)
			{
				return 0;
			}

			template<typename T, typename ...Args>
			static size_t dump_args(char* buf, T arg, Args... code_args)
			{
				char * old_buf = buf;
				buf += dump_one_arg(buf, arg);
				buf += dump_args(buf, code_args...);
				return buf - old_buf;
			}

			template<typename ...Args>
			size_t dump_byte(char* buf, BytecodeEnum code_enum, Args... code_args)
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
}