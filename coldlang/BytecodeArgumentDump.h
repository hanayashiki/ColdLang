#pragma once
#include "stdafx.h"
#include "BytecodeByteDump.h"
#include "NativeFunction.h"

#define ARG_INFO(Type, size_expr, dump_block, retrieve_block) \
	template<> \
	struct get_one_length<Type> \
	{\
		static constexpr size_t value = (size_expr);\
	};\
	static size_t dump_one_arg(unsigned char * buf, Type src) { dump_block return get_one_length<Type>::value;} \
	template<>\
	static Type retrieve_arg<Type>(unsigned char * buf) retrieve_block

namespace IR
{
	namespace BytecodeClass
	{
		static string mem_to_string(unsigned char * start, size_t len);

		using namespace OperandType;

		template<typename T>
		struct get_one_length
		{
			static constexpr size_t value = 0;
		};
		template<typename T>
		static T retrieve_arg(unsigned char * buf)
		{
			return T::this_fucking_not_exist;
		}

		//static size_t dump_one_arg(char* buf, NativeFunction* native_func)
		//{
		//	memcpy(buf, &native_func, sizeof(native_func));
		//	return sizeof(native_func);
		//}
		//template<>
		//struct get_one_length<NativeFunction*>
		//{
		//	static constexpr size_t value = sizeof(NativeFunction*);
		//};

		ARG_INFO(NativeFunction*, sizeof(NativeFunction*), 
			{
				memcpy(buf, &src, sizeof(src));
			},
			{
				return reinterpret_cast<NativeFunction*>(*(size_t*)buf);
			}
		)

		ARG_INFO(Literal*, sizeof(Literal*),
			{
				memcpy(buf, &src, sizeof(src));
			},
			{
				return reinterpret_cast<Literal*>(*(size_t*)buf);
			}
		)

		ARG_INFO(Variable*, sizeof(Variable*),
			{
				memcpy(buf, &src, sizeof(src));
			},
			{
				return reinterpret_cast<Variable*>(*(size_t*)buf);
			}
		)

		ARG_INFO(Constant*, sizeof(Constant*),
			{
				memcpy(buf, &src, sizeof(src));
			},
			{
				return reinterpret_cast<Constant*>(*(size_t*)buf);
			}
		)

		ARG_INFO(Label*, sizeof(Label*),
			{
				memcpy(buf, &src, sizeof(src));
			},
			{
				return reinterpret_cast<Label*>(*(size_t*)buf);
			}
		)

	}
}