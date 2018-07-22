#pragma once
#include "stdafx.h"

namespace IR
{
	namespace OperandType
	{
		class Label {
		private:
			static int id;
			wstring name_;
		public:
			explicit Label(wstring && name);
			wstring to_string();
		};
	}
}