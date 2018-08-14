#pragma once
#include "stdafx.h"

namespace IR
{
	namespace OperandType
	{
		class Label {
		private:
			static uint32_t global_id_;
			static vector<Label> label_table_;
			uint32_t id_;
			wstring name_;
		public:
			typedef uint32_t id_type;
			explicit Label(wstring && name);
			Label(const Label &);
			Label(Label &&) noexcept;
			Label & operator=(Label &&) noexcept;
			Label & operator=(const Label & label);
			wstring to_string();
			const uint32_t & get_id();
			static Label & id_to_label(uint32_t id);

		};
	}
}