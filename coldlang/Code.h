#pragma once

namespace Compile
{

	class Code
	{
	public:
		typedef void* FuncPtr;
	private:
		FuncPtr func_ptr_;
	public:
		explicit Code(FuncPtr func_ptr)
			: func_ptr_(func_ptr)
		{
		}
		~Code();

		void operator() ()
		{
			// TODO: param?
			reinterpret_cast<void(*)()>(func_ptr_)();
		}
	};
}
