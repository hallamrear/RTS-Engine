#define MAX_LIGHTS 4

struct Light
{
	vec4 RGBI;
	vec4 Position;
};

layout(binding = 0) uniform UniformBufferObject
{
	mat4 View;
	mat4 Projection;
	Light LightData[MAX_LIGHTS];
} UBO;

layout(binding = 1) uniform sampler2D texSampler;

layout(push_constant) uniform PushConstants
{
	mat4 Model;
	vec4 CameraPosition;
} PC;

vec3 CalculateLighting(Light lightData, vec3 fragPosition, vec3 fragNormal)
{
	if (lightData.RGBI.w == 0.0f)
	{
		return vec3(0.0f);
	}

	vec3 lightDirection = normalize(lightData.Position.xyz - fragPosition);
	vec3 lightColour = lightData.RGBI.xyz;
	float lightIntensity = lightData.RGBI.w;

	vec3 ambient = 0.05f * lightColour;

	vec3 diffuse = max(dot(normalize(fragNormal), lightDirection), 0.0) * lightColour;

	float shininess = 32.0f; 
	float specularStrength = 0.1;
	vec3 viewDir = normalize(PC.CameraPosition.xyz - fragPosition);
	vec3 reflectDir = normalize(reflect(-lightDirection, fragNormal));
	float highlight = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = specularStrength * highlight * lightColour;

	return vec3(diffuse + ambient + specular);
}