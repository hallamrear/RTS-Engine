#version 450
#extension GL_ARB_separate_shader_objects : enable
#include "ShaderStructs.glsl"

layout(location = 0) in vec4 colour;
layout(location = 1) in vec3 fragPosition;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 UV;

layout(location = 0) out vec4 outColor;

void main()
{
	vec4 texColour = texture(texSampler, UV);
	vec3 lightSum = vec3(0.0f);

	for(int i = 0; i < MAX_LIGHTS; i++)
	{
		lightSum += CalculateLighting(UBO.LightData[i], fragPosition, normal);	
	}
	
	vec3 result = lightSum * texColour.xyz;
    outColor = vec4(result, 1.0);
}