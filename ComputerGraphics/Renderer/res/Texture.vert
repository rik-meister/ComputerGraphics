#version 430 core

// vertex attributes (data from the model) (in the VBO)
layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 uv;

out vUV;

// shader program uniforms
layout (location = 0) uniform mat4 proj;
layout (location = 1) uniform mat4 view;
layout (location = 2) uniform mat4 model;

void main()
{
    vUV = uv;

    gl_Position  = proj * view * model * position;
}