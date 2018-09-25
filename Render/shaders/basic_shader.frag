#version 430
layout(location = 3) uniform sampler2D albedo;

in vec4 vColor;
in vec2 vUV;
in vec3 vNormal;

out vec4 vertColor;
void main() 
{ 
vertColor = texture (albedo, vUV); 
};