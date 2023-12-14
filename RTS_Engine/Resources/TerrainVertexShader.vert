#version 450

int terrainWidth = 50;
int chunkCount = terrainWidth * terrainWidth;

layout(binding = 0) uniform UniformBufferObject
{
	mat4 View;
	mat4 Projection;
	vec2 ChunkPositions[2500]; /* EQUAL TO CHUNK COUNT */
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
	
	vec3 sumPosition = vec3(gridPosition.x + chunkPosition.x, gridPosition.y, gridPosition.z + chunkPosition.y);
	vec4 worldPosition = vec4(sumPosition, 1.0f) * PC.Model;
	
	float fullSize = (terrainWidth * 8);

	UV.x = sumPosition.x / fullSize;
	UV.y = sumPosition.z / fullSize;
	vec4 sampledColour = texture(texSampler, UV);
	sumPosition.y = sampledColour.r * 20.0f;			
	
	outPosition = worldPosition.xyz;
	mat4 MVP =	UBO.Projection * UBO.View * PC.Model;
	gl_Position = MVP * vec4(sumPosition, 1.0f);

    //gl_VertexIndex
	//gl_InstanceIndex	
}
