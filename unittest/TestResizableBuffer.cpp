#include "stdafx.h"
#include "CppUnitTest.h"

#include "../coldlang/stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unittest
{
	TEST_CLASS(TestResizableBuffer)
	{
	public:

		TEST_METHOD(TestPushOne)
		{
			ResizableBuffer<wchar_t> resizable_buffer(1);
			resizable_buffer.push(L'我');
			Assert::AreEqual(L"我", resizable_buffer.get_null_terminated_buf());
			resizable_buffer.free_buf();
		}
		TEST_METHOD(TestPushTen)
		{
			ResizableBuffer<wchar_t> resizable_buffer(1);
			for (int i = 0; i < 10; i++) {
				resizable_buffer.push(L'我');
			}
			Assert::AreEqual(L"我我我我我我我我我我", resizable_buffer.get_null_terminated_buf());
			resizable_buffer.free_buf();
		}
		TEST_METHOD(TestDeletedAndBufExists)
		{
			auto resizable_buffer = new ResizableBuffer<wchar_t>(1);
			for (int i = 0; i < 16; i++) {
				resizable_buffer->push(L'我');
			}
			auto get_nullt_wstring = resizable_buffer->get_null_terminated_buf();
			delete(resizable_buffer);
			Assert::AreEqual(L"我我我我我我我我我我我我我我我我", get_nullt_wstring);
		}
	};
}