#version 450

layout(binding = 0) uniform UniformBufferObject
{
	mat4 View;
	mat4 Projection;
} UBO;

layout(binding = 1) uniform sampler2D texSampler;

layout(push_constant) uniform PushConstants
{
	mat4 Model;
} PC;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;
layout(location = 3) in vec4 inColour;

layout(location = 0) out vec3 outColour;
layout(location = 1) out vec2 outUV;

void main()
{
	mat4 MVP =	UBO.Projection * UBO.View * PC.Model;
	gl_Position = MVP * vec4(inPosition, 1.0f);
	outColour = vec4(inPosition, 1.0f).rgb;
	outUV = inUV;
}