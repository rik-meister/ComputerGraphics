#version 430 core

in vec2 vUV;
in vec3 vNormal;

out vec4 outColor;

// shader program uniforms
layout (location = 3) uniform sampler2D albedo;
layout (location = 4) uniform vec3 ambientLight = vec3(0.2, 0.2, 0.2);
layout (location = 5) uniform vec3 lightDir;

void main()
{
    // texture color
    vec4 baseColor = texture(albedo, vUV);

    // directional lighting
    float d = max(0, dot(vNormal, -lightDir));
    vec3 diffuse = vec3(d); // vec(d,d,d)

    vec3 lighting = ambientLight + diffuse;

    outColor.rgb = baseColor.rgb * lighting;
    outColor.a = baseColor.a;
}
