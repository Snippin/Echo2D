#version 450 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aUV;

out vec4 fragColor;
out vec2 fragUV;

uniform mat4 uProjection;

void main()
{
	gl_Position = uProjection * vec4(aPos, 0.0f, 1.0f);
	
	fragColor = aColor;
	fragUV = aUV;
}
