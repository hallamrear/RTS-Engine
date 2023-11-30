#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject
{
	mat4 View;
	mat4 Projection;
} UBO;

layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform texture2D textures[64];

layout(location = 0) in vec4 colour;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 UV;
layout(location = 0) out vec4 outColor;

layout(push_constant) uniform PushConstants
{
	mat4 Model;
	int texID;
	int padd[3];
} PC;

void main()
{
	outColor = texture(texSampler, UV);
}