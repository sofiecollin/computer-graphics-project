#version 150

out vec4 out_Color;
uniform sampler2D tex;
in vec2 texCoord;
in vec4 particle_color;

void main(void)
{
	out_Color = particle_color*texture(tex, texCoord);
}