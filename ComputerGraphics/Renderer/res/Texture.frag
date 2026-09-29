#version 430 core

in vec2 vUV

out vec4 outColor;

// shader program uniforms
layout (location = 3) uniform sampler 2D albedo;
void main()
{
    outColor = texture(albedo, vUV);
}
