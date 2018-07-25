#pragma once

namespace IR
{
	class CLObject
	{
	private:
		size_t id_;
	public:
		CLObject();
		virtual wstring to_string() = 0;
		void set_id(size_t id);
		size_t get_id();
		~CLObject();
	};
}