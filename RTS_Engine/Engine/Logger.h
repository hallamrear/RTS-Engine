#pragma once
#include "BennettPCH.h"
#include <debugapi.h>

enum LOG_STATUS
{
	LOG_SAFE = 2,
	LOG_MINIMAL = 6,
	LOG_SERIOUS = 12,
	LOG_CRITICAL = 4,
	LOG_SPECIAL = 13
};

#ifdef _DEBUG
template<typename T>
inline void Log(T var, int statusLevel)
{
	//Changing debug colour then back to original.
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)statusLevel);
	std::cout << var << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	
	if (statusLevel == LOG_CRITICAL)
		DebugBreak();
};
#else
template<class T>
inline void Log(T var, int statusLevel) 
{

};
#endif