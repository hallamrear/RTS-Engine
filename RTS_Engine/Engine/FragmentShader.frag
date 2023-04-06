#version 450

layout(location = 0) out vec4 outColor;
layout(location = 1) in vec3 colour;
layout(location = 2) in vec2 Position;

void main()
{
    outColor = vec4(colour.r, colour.g, colour.b, 1.0f);
}