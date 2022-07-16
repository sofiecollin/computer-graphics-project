#version 150
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_explicit_attrib_location : require

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 delta_pos;
layout(location=2) in vec4 color;

uniform mat4 mdlMatrix;
uniform mat4 projMatrix;

out vec2 texCoord;
out vec4 particle_color;

void main(void)
{
	texCoord.s = in_Position.x+0.5;
	texCoord.t = in_Position.y+0.5;
	particle_color = color;
	gl_Position = projMatrix * mdlMatrix *  vec4((in_Position+delta_pos), 1.0);
}