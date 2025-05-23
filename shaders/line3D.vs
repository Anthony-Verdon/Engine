#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 projection;
uniform mat4 view;

out vec4 color;
void main()
{
    gl_Position = projection * view * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    color = aColor;
}