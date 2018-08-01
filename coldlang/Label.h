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
			Label(const Label &);
			Label(Label &&) noexcept;
			Label & operator=(Label &&) noexcept;
			Label & operator=(const Label & label);
			wstring to_string();
		};
	}
}