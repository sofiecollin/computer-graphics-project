#version 150

in vec3 in_Position;
in vec2 in_TexCoord;

out vec2 texCoords;

uniform mat4 camMatrix;
uniform mat4 projMatrix;

void main(void)
{
	gl_Position = projMatrix * camMatrix * vec4(in_Position, 1.0);
    texCoords = in_TexCoord;
}