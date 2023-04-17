#version 450

layout(binding = 0) uniform UniformBufferObject
{
	mat4 Model;
	mat4 View;
	mat4 Projection;
} UBO;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;
layout(location = 3) in vec4 inColour;

layout(location = 0) out vec3 outColour;

void main()
{
	mat4 MVP =	UBO.Projection * UBO.View * UBO.Model;
	gl_Position = MVP * vec4(inPosition, 1.0f);
	outColour = (MVP * vec4(inNormal, 1.0f)).rgb;
}