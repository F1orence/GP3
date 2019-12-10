#include "pch.h"
#include "Log.h"

void Log::Debug(const std::string& msg, const char* file, int line) // error
{
		std::cout << "LOG DEBUG(" << file << ", " << line << "): " << msg << std::endl;
}

void Log::Debug(const std::string& msg, const char* file, int line, int mode) // error
{
	if (mode != HIDE_ALL)
	{
		std::cout << "LOG DEBUG(" << file << ", " << line << "): " << msg << std::endl;
		LOG_DEBUG("I'm Updating!");
	}
}

void Log::Debug(const std::string& msg, int mode) // trace
{
	if (mode != ERRORS_ONLY) 
	{ 
		std::cout << "Debug: " << msg << std::endl; 
	}
}

/*void Log::Trace(const std::string& msg)
{
	std::cout << msg << std::endl;
}*/


bool Log::CheckGLError(const char* file, int line)
{
	GLenum err;
	bool errorFound = false;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cout << "GL ERROR(" << file << ", " << line << "): " <<
			glewGetErrorString(err) << std::endl;
		errorFound = true;
	}
	return errorFound;
}