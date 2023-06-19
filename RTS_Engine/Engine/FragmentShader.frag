#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler texSampler;
layout(binding = 2) uniform texture2D textures[64];

layout(location = 0) in vec3 colour;
layout(location = 1) in vec2 UV;
layout(location = 0) out vec4 outColor;

layout(push_constant) uniform PushConstants
{
	mat4 Model;
	int texID;
	int padd[3];
} PC;

void main()
{
	outColor = texture(sampler2D(textures[PC.texID], texSampler), UV);
}