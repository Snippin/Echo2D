#version 450 core

in vec2 fragUVs;
in vec4 fragColor;
in float fragThickness;

out vec4 color;

void main()
{
    float distance = 1 - length(fragUVs); // length calculates length of the given vector
    float width = fwidth(distance); // fwidth - returns the sum of absolute value of derivatives in x and y
    float circle = smoothstep(0, width, distance); // smoothstep will interpolate between the two values

    circle *= smoothstep(fragThickness + width, fragThickness, distance);
    
    if (circle == 0.0)
	{
        discard;
	}

    color = fragColor;
    color.a *= circle;
}
