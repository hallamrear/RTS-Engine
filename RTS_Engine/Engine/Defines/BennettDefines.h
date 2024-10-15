#pragma once

//System Defines
#define TIMESTEP_CAP 0.32f
#define WINDOW_WIDTH 1280.0f
#define WINDOW_HEIGHT 720.0f
//When changing this value, be sure to change the hard-coded version in any shaders.
#define MAX_LOADED_TEXTURES 64
#define MAX_LOADSTRING 100

//Engine Defines
#define BENNETT_LEFT_VECTOR		glm::vec3(+1.0f, 0.0f, 0.0f)
#define BENNETT_UP_VECTOR		glm::vec3( 0.0f,+1.0f, 0.0f)
#define BENNETT_FORWARD_VECTOR	glm::vec3( 0.0f, 0.0f,+1.0f)
#define BENNETT_RIGHT_VECTOR	glm::vec3(-1.0f, 0.0f, 0.0f)
#define BENNETT_DOWN_VECTOR		glm::vec3( 0.0f,-1.0f, 0.0f)
#define BENNETT_BACK_VECTOR		glm::vec3( 0.0f, 0.0f,-1.0f)

//Physics and Collision defines.
#define COLLISION_EPA_ITERATION_CAP 100
#define RIGIDBODY_DEFAULT_MASS 100
#define PHYSICS_GRAVITY_FORCE glm::vec3(0.0f, -9.81f, 0.0f)

//Renderer Defines
#define MAX_DEBUG_LINE_COUNT 100
#define MAX_DEBUG_CIRCLE_COUNT 100
#define DEBUG_CIRCLE_POINT_COUNT 12
//Lighting Defines
#define MAX_DIRECTIONAL_LIGHT_COUNT 2
#define MAX_POINT_LIGHT_COUNT 2
#define MAX_LIGHT_COUNT MAX_POINT_LIGHT_COUNT + MAX_DIRECTIONAL_LIGHT_COUNT

//Custom Defines
#ifdef ENGINE_EXPORTS
#define BENNETT_ENGINE __declspec(dllexport)
#else
#define BENNETT_ENGINE __declspec(dllimport)
#endif

#define WIN32MSG UINT

typedef signed short s16;
typedef unsigned short u16;
typedef unsigned short ushort;

typedef signed int s32;
typedef unsigned int u32;
typedef unsigned int uint;

typedef unsigned char uchar;

typedef unsigned short VertexIndex;