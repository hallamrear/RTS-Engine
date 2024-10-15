#version 450
#include "ShaderStructs.glsl"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;
layout(location = 3) in vec4 inColour;

layout(location = 0) out vec4 outColour;
layout(location = 1) out vec3 outFragPosition;
layout(location = 2) out vec3 outNormal;
layout(location = 3) out vec2 outUV;

void main()
{
	mat4 MVP =	UBO.Projection * UBO.View * PC.Model;
	gl_Position = MVP * vec4(inPosition, 1.0f);
	outColour = vec4(inColour);

	outFragPosition = vec3(PC.Model * vec4(inPosition, 1.0));

	vec4 wsNormal = vec4(inNormal, 1.0f) * (PC.Model);
	outNormal = wsNormal.rgb;

	outNormal = mat3(transpose(inverse(PC.Model))) * inNormal;

	outUV = inUV;
}