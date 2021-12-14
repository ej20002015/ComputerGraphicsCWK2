#include "Log.h"

void Log::message(const std::string& message)
{
	std::cout << "MESSAGE: " << message << std::endl;
}

void Log::fatal(const std::string& message)
{
	std::cout << "FATAL: " << message << std::endl;
	std::cin.get();
	exit(1);
}
