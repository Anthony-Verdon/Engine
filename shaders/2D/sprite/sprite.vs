#version 330 core
layout (location = 0) in vec2 positions;
layout (location = 1) in vec2 texturesCoords;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 model;

void main()
{
    TexCoords = texturesCoords;
    gl_Position = projection * model * vec4(positions, 0.0, 1.0);
}