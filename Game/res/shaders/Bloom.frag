#version 330

uniform sampler2D renderedColor;

in vec2 fragmentTexcoord;

layout(location = 0) out vec3 pixelColor;

vec3 blurredColor(int radius, float scale)
{
	vec3 avarage = vec3(0.0);
	int counter = 0;
	vec2 texelSize = 1.0 / textureSize(renderedColor, 0);
	for (int x = -radius; x <= radius; x++) {
		for (int y = -radius; y <= radius; y++) {
			avarage += texture(renderedColor, fragmentTexcoord + vec2(x, y) * texelSize * scale).rgb;
			counter++;
		}
	}
	return avarage / float(counter);
}

void main()
{
	vec3 color = blurredColor(2, 4.0);
	color = color * color;
	pixelColor = color;
}