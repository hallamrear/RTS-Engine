#pragma once

//I am disabling this error to avoid all the messages for DLL exports from Vulkan objects.
#pragma warning( disable : 4251)

#include <Defines/BennettDefines.h>

//Windows Libraries
#include <External/WindowsHeaders.h>

//GLM
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

//C++ Libraries
#include <array>
#include <iostream>
#include <string>
#include <cmath>
#include <functional>
#include <vector>
#include <algorithm>

//Adding Logger
#include <System/Logger.h>