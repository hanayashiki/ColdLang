#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <ctime>
#include <cctype>

#define CLD_LEVEL_VERBOSE 0
#define CLD_LEVEL_DEBUG 1
#define CLD_LEVEL_INFO 2
#define CLD_LEVEL_ERR 3
#define CLD_LEVEL_FATAL 4

#define FILTER_LEVEL 3

#define _CLD_LOG(level, levelName) if (level >= FILTER_LEVEL) std::wcerr << "[" << levelName << "] " << __FUNCTION__ << "@[" << \
			__get_filename(__FILE__).c_str() << ", " << __LINE__ << "] "

#define CLD_INFO _CLD_LOG(CLD_LEVEL_INFO, "INFO")

#define CLD_DEBUG _CLD_LOG(CLD_LEVEL_DEBUG, "DEBUG")

#define CLD_WARN _CLD_LOG(CLD_LEVEL_INFO, "WARN")
#define CLD_ERROR _CLD_LOG(CLD_LEVEL_ERR, "ERROR")
#define CLD_FATAL _CLD_LOG(CLD_LEVEL_FATAL, "FATAL")

#define LOG_EXPR(x) #x << L" = " << (x) 

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