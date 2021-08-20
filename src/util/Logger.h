#pragma once

#include <iostream>

class Logger
{
public:
	static inline void LOG_INFO(const char* msg)	{ std::cout << "[INFO] : " << msg << std::endl; }
	static inline void LOG_WARNING(const char* msg) { std::cout << "[WARNING] : " << msg << std::endl; }
	static inline void LOG_ERROR(const char* msg)	{ std::cout << "[ERROR] : " << msg << std::endl; }
private:
	Logger();
};