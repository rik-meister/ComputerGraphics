#version 430 core

in vec4 vColor;

out vec4 outColor;

void main()
{
    //outColor = vec4(1.0, 0.0, 0.0, 1.0);
    outColor = vColor;
}
