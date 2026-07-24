#version 450 core

in vec2 fragUVs;
in vec4 fragColor;

uniform sampler2D uTex;

out vec4 color;

void main()
{
	vec4 textureColor = texture(uTex, fragUVs);
	color = textureColor * fragColor;
}