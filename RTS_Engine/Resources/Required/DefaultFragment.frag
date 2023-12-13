#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject
{
	mat4 View;
	mat4 Projection;
	vec2 ChunkPositions[64];
} UBO;

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec4 colour;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 UV;
layout(location = 0) out vec4 outColor;

layout(push_constant) uniform PushConstants
{
	mat4 Model;
} PC;


void main()
{
	outColor = texture(texSampler, UV);
}