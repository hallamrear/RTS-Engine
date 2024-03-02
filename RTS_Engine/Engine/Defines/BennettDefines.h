#pragma once

//System Defines
#define TIMESTEP_CAP 1.0f
#define WINDOW_WIDTH 1280.0f
#define WINDOW_HEIGHT 720.0f
#define COLLISION_EPA_ITERATION_CAP 100
//When changing this value, be sure to change the hard-coded version in any shaders.
#define MAX_LOADED_TEXTURES 64
#define MAX_LOADSTRING 100

//Custom Defines
#define BENNETT_ENGINE __declspec(dllexport)
#define WIN32MSG UINT

typedef signed short s16;
typedef unsigned short u16;
typedef unsigned short ushort;

typedef signed int s32;
typedef unsigned int u32;
typedef unsigned int uint;

typedef unsigned char uchar;

typedef unsigned short VertexIndex;