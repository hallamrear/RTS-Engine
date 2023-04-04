#pragma once

//Windows Libraries
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif // _WIN32


//C++ Libraries
#include <iostream>
#include <string>
#include <cmath>
#include <functional>
#include <vector>
#include <algorithm>

#ifdef _DEBUG
#include "Logger.h"
#else
#define void Log(T var, int statusLevel) {};
#endif // _DEBUG