#pragma once

//I am disabling this error to avoid all the messages for DLL exports from Vulkan objects.
#pragma warning( disable : 4251)

#include <Defines/BennettDefines.h>

//Windows Libraries
#include <External/WindowsHeaders.h>

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
#include <System/Logger.h>