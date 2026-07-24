#version 450 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec4 aColor;

uniform mat4 uProjection;

out vec2 fragUVs;
out vec4 fragColor;

void main()
{
	gl_Position = uProjection * vec4(aPos, 0.0f, 1.0f);
	fragUVs = aTexCoords;
	fragColor = aColor;
}