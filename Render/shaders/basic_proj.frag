#version 430
layout (location = 3) uniform sampler2D albedo;
layout (location = 4) uniform vec3 lightDir;

in vec4 vColor;
in vec2 vUV;
in vec3 vNormal;

out vec4 outColor;

void main() { 
    float diffuse = max(.5f,dot(vNormal, -lightDir));
    outColor = texture(albedo, vUV);
    outColor.xyz = outColor.xyz * diffuse;
};