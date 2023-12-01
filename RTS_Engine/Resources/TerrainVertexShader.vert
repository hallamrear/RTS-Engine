#version 450
#extension GL_EXT_debug_printf : enable

layout(binding = 0) uniform UniformBufferObject
{
	mat4 View;
	mat4 Projection;
} UBO;

layout(push_constant) uniform PushConstants
{
	mat4 Model;
	float dt;
	float padding[3];
} PC;

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 details;

layout(location = 0) out vec3 outPosition;
layout(location = 1) out vec2 UV;

float VERTICES_PER_RUN = 20.0f;
float CLAMPED_VERTICES_PER_RUN = 17.0f;

void main()
{
	float rowIndex = mod(gl_VertexIndex, VERTICES_PER_RUN);
	float clampedIndex = clamp(rowIndex - 1.0f, 0.0f, CLAMPED_VERTICES_PER_RUN);
	
	vec3 position = vec3(0.0f, 0.0f, 0.0f);
	position.x = floor(clampedIndex / 2.0f);
	position.z = mod(clampedIndex, 2.0f);
	position.z += floor(gl_VertexIndex / VERTICES_PER_RUN);
	
	UV.x = outPosition.x / 8.0f;
	UV.y = outPosition.z / 8.0f;
	vec4 sampledColour = texture(texSampler, UV);
	position.y = sampledColour.r * 10;
	
	outPosition = position;
	
	mat4 MVP =	UBO.Projection * UBO.View * PC.Model;
	gl_Position = MVP * vec4(outPosition, 1.0f);
	
    //gl_VertexIndex
	//gl_InstanceIndex
	
}
