#pragma once

//Windows Libraries
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32


//C++ Libraries
#include <iostream>
#include <string>
#include <cmath>
#include <functional>

#ifdef _DEBUG
#include "Logger.h"
#else
#define void Log(T var, int statusLevel) {};
#endif // _DEBUG