#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in float ao;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 TexCoords;
out float aAo;

void main()
{
    aAo= ao;
    gl_Position = projection*view*model*vec4(aPos,1.0f);
    TexCoords = aTexCoords;
}
