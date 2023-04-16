#version 450

layout(location = 0) in vec3 colour;
layout(location = 0) out vec4 outColor;

void main()
{
    outColor = vec4(colour.r, colour.g, colour.b, 1.0f);
}