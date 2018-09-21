#pragma once
#include "stdafx.h"
#include "NativeSymbol.h"
#include "RuntimeObject.h"

namespace IR
{
	namespace OperandType
	{
		class NativeFunction : public NativeSymbol
		{
			typedef CldRuntime::ValueType ValueType;
		private:
			void * func_ptr_;
			ValueType ret_type_;
			std::vector<ValueType> param_types_;
		public:
			NativeFunction(const wchar_t * name, void * func_ptr, std::initializer_list<ValueType> signature)
				: func_ptr_(func_ptr), NativeSymbol(name), ret_type_(*signature.begin()),
				param_types_(signature.begin() + 1, signature.end())
			{
				//wcout << name << "to native function : " << (long long int)func_ptr << endl;
			} 

			void * get_func_ptr()
			{
				return func_ptr_;
			}

			ValueType get_ret_type()
			{
				return ret_type_;
			}

			const std::vector<ValueType> & get_param_types()
			{
				return param_types_;
			}
		};
	}
}
