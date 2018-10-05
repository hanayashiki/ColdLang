#pragma once
#include <vector>
#include <string>

namespace CldUtils
{
	template<typename Cr>
	std::wstring Join(const std::wstring & sep, const Cr & cr)
	{
		std::wstring res(L"");
		if (cr.size() == 0)
		{
			return res;
		}
		for (auto & str :  cr)
		{
			res += str;
			res += sep;
		}
		return res.substr(0, res.size() - sep.size());
	}

	template<typename Cr>
	std::wstring Embraced(const wchar_t * left, const wchar_t * right, const std::wstring & sep, const Cr & cr)
	{
		return std::wstring(left) + Join(sep, cr) + std::wstring(right);
	}

	template<typename Cr>
	std::wstring List(const Cr & cr)
	{
		return Embraced(L"[", L"]", L", ", cr);
	}

	template<typename Cr>
	std::wstring Set(const Cr & cr)
	{
		return Embraced(L"{", L"}", L", ", cr);
	}

	inline
	int32_t Align16(int32_t n)
	{
		return n + (0xF & (~(0xF & n) + 1));
	}

	inline
	int32_t Align8(int32_t n)
	{
		return n + (0x7 & (~(0x7 & n) + 1));
	}

	inline
	int32_t Align4(int32_t n)
	{
		return n + (0x3 & (~(0x3 & n) + 1));
	}

	static string MemToString(unsigned char * start, size_t len)
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
}