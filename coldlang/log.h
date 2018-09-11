#pragma once

#include <iostream>
#include <string>
#include <ctime>
#include <cctype>

#define _CLD_LOG(x) std::wcerr << "[" << #x << "] " << __FUNCTION__ << "@[" << \
			__get_filename(__FILE__).c_str() << ", " << __LINE__ << "] "

#define CLD_INFO _CLD_LOG(INFO)

#define CLD_DEBUG _CLD_LOG(DEBUG)

#define CLD_WARN _CLD_LOG(WARN)
#define CLD_ERROR _CLD_LOG(DEC_ERROR)
#define CLD_FATAL _CLD_LOG(FATAL)

static inline std::string __now() {
	time_t now = time(0);
	std::string date = ctime(&now);
	for (int i = date.length() - 1; i >= 0; i--) {
		if (std::isspace(date[i])) {
			date.pop_back();
		}
		else {
			return date;
		}
	}
}

static inline std::string __get_filename(std::string path) {
	int i;
	for (i = path.length() - 1; i >= 0; i--) {
		if (path[i] == '/' || path[i] == '\\') {
			break;
		}
	}
	return path.substr(i + 1);
}