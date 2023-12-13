#version 450

int chunkCount = 4096;

layout(binding = 0) uniform UniformBufferObject
{
	mat4 View;
	mat4 Projection;
	vec2 ChunkPositions[4096];
} UBO;

layout(push_constant) uniform PushConstants
{
	mat4 Model;
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
	
	vec3 gridPosition = vec3(0.0f, 0.0f, 0.0f);
	gridPosition.x = floor(clampedIndex / 2.0f);
	gridPosition.z = mod(clampedIndex, 2.0f);
	gridPosition.z += floor(gl_VertexIndex / VERTICES_PER_RUN);
	
	vec2 chunkPosition = UBO.ChunkPositions[gl_InstanceIndex];
	
	vec3 position = vec3(gridPosition.x + chunkPosition.x,	gridPosition.y, gridPosition.z + chunkPosition.y);
	vec4 wPos = vec4(position, 1.0f) * PC.Model;
	position = wPos.xyz;
	
	UV.x = position.x / chunkCount;
	UV.y = position.z / chunkCount;
	vec4 sampledColour = texture(texSampler, UV);
	position.y = sampledColour.r * 20.0f;			
	
	//UV = chunkPosition;

	outPosition = wPos.xyz;
	mat4 MVP =	UBO.Projection * UBO.View * PC.Model;
	gl_Position = MVP * vec4(position, 1.0f);
	
    //gl_VertexIndex
	//gl_InstanceIndex
	
}
