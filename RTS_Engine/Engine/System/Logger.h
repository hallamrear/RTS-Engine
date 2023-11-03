#pragma once
#include <BennettPCH.h>
#include <Defines/DebugDefines.h>
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
//Todo : Rearrange parameters to match other log function. Ensure release define function also matches.
inline BENNETT_ENGINE void Log(T var, LOG_STATUS status)
{
	if (var == "" || !ENABLE_LOG)
		return;

	//Changing debug colour then back to original.
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)statusLevel);
	std::cout << var << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	
	if (statusLevel == LOG_CRITICAL)
		DebugBreak();
};

inline BENNETT_ENGINE void Log(LOG_STATUS statusLevel, const char* format, ...)
{
	if (format == "" || !ENABLE_LOG)
		return;

	va_list args;
	//Changing debug colour then back to original.
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)statusLevel);
	va_start(args, format);
	printf(format, args);
	va_end(args);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

	if (statusLevel == LOG_CRITICAL)
		DebugBreak();
}

inline BENNETT_ENGINE std::string GetLastWin32Error()
{
	DWORD error = GetLastError();
	if (error)
	{
		LPVOID lpMsgBuf{};
		DWORD bufLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			error,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);
		if (bufLen)
		{
			LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
			std::string result(lpMsgStr, lpMsgStr + bufLen);

			LocalFree(lpMsgBuf);

			return result;
		}
	}

	return "";
};

#else
template<class T>
inline void BENNETT_ENGINE Log(T var, int statusLevel)
{

};

inline BENNETT_ENGINE std::string BENNETT_ENGINE GetLastWin32Error()
{
	return "";
};


inline BENNETT_ENGINE void Log(LOG_STATUS statusLevel, const char* format, ...)
{

}

#endif