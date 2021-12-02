#pragma once

#include <iostream>

class Log
{
public:
	Log() = delete;

	static void message(const std::string& message);
	static void fatal(const std::string& message);
};