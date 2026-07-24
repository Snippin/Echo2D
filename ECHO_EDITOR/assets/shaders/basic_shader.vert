#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
uniform mat4 uProjection;

out vec2 fragUVs;

void main()
{
	gl_Position = uProjection * vec4(aPos, 1.0f);
	fragUVs = aTexCoords;
}