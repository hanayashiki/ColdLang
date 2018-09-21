#pragma once

#include <string>
#include <sstream>

#include "RuntimeValue.h"
#include "log.h"
#include "../include/nlohmann_json/json.hpp"

namespace CldRuntime
{
	static std::string ToHex(uint64_t integer)
	{
		std::stringstream stream;
		stream << std::hex << integer;
		std::string result(stream.str());
		return result;
	}

	static std::string ToString(RuntimeValue * value)
	{
		using namespace nlohmann;
		json j;
		j["type"] = ValueTypeName[value->type];
		
		CLD_DEBUG << LOG_EXPR(value);

		switch (value->type)
		{
		case StringPointerVal:
			j["value"] = ToHex((uint64_t)reinterpret_cast<PointerValue*>(value)->value);
			break;
		}

		return j.dump();
	}

}