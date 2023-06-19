#pragma once

//Windows Libraries
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif // _WIN32

//C++ Libraries
#include <array>
#include <iostream>
#include <string>
#include <cmath>
#include <functional>
#include <vector>
#include <algorithm>

//GLM
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Adding Logger
#include "Logger.h"

//System Defines
#define TIMESTEP_CAP 0.2f
#define WINDOW_WIDTH 1280.0f
#define WINDOW_HEIGHT 720.0f

//When changing this value, be sure to change the hard-coded version in any shaders.
#define MAX_LOADED_TEXTURES 64