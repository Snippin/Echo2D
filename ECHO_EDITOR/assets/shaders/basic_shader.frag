#version 450 core

in vec2 fragUVs;
uniform sampler2D uTex;

out vec4 color;

void main()
{
	color = texture(uTex, fragUVs);
}