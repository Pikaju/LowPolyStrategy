#version 330

uniform sampler2D fowTexture;
uniform vec2 chunkPosition;

in vec3 fragmentPosition;
in vec3 fragmentNormal;
in vec3 fragmentColor;

layout(location = 0) out vec4 pixelColor;
layout(location = 1) out vec3 pixelNormal;
layout(location = 2) out float pixelDepth;

void main()
{
	float fow = texture(fowTexture, 1.0 / 34.0 * (fragmentPosition.xz - chunkPosition + 1.0)).r;
	float light = dot(-fragmentNormal, normalize(vec3(1.0, -1.0, 0.0))) * 0.5 + 0.5;
	pixelColor = vec4(fragmentColor * light * fow, 1.0);
	pixelNormal = fragmentNormal * 0.5 + 0.5;
	pixelDepth = gl_FragCoord.z;
}