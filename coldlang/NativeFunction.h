#pragma once
#include "stdafx.h"
#include "NativeSymbol.h"

namespace IR
{
	namespace OperandType
	{
		class NativeFunction : public NativeSymbol
		{
		private:
			void * func_ptr_;
		public:
			NativeFunction(const wchar_t * name, void * func_ptr)
				: func_ptr_(func_ptr), NativeSymbol(name)
			{
				//wcout << name << "to native function : " << (long long int)func_ptr << endl;
			} 

			void * get_func_ptr()
			{
				return func_ptr_;
			}

		};
	}
}
