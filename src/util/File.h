#pragma once
#include <iostream>
#include <fstream>
#include <sstream>

namespace File
{
	std::string Read(const char* path)
	{
		std::ifstream pathStream;
		std::stringstream buffer;

		pathStream.exceptions((std::ifstream::failbit | std::ifstream::badbit));

		try
		{
			pathStream.open(path);

			buffer << pathStream.rdbuf();

			pathStream.close();

			return buffer.str();
		}
		catch (std::ifstream::failure& e)
		{
			return nullptr;
		}
	}
}