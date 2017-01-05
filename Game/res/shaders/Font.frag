#version 330

uniform sampler2D fontTexture;
uniform float scale;
uniform vec4 color;

in vec2 fragmentTexcoord;

out vec4 pixelColor;

void main()
{
	const float thickness = 0.5;
	float aaThickness = 0.1 / scale;
	vec4 sample = texture(fontTexture, fragmentTexcoord);
	float distance = 1.0 - sample.a;
	float alpha = 1.0 - smoothstep(thickness, thickness + aaThickness, distance);

	pixelColor = vec4(color.rgb, alpha * color.a);
}