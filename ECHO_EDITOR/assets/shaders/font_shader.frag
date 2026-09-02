#version 450 core

in vec4 fragColor;
in vec2 fragUV;

out vec4 color;

uniform sampler2D uAtlas;

void main()
{
    vec4 pixelColor = vec4(1, 1, 1, texture(uAtlas, fragUV).r);

	color = pixelColor * fragColor;
}
