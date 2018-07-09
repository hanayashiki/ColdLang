#pragma once
#include "stdafx.h"

class BadSyntaxException : public std::exception
{
public:
	BadSyntaxException(const char * message) : std::exception(message) {}
};