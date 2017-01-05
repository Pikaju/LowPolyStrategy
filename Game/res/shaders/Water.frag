#version 330

uniform vec3 cameraPosition;
uniform vec2 chunkPosition;
uniform sampler2D fowTexture;

in vec3 fragmentPosition;
flat in vec3 fragmentNormal;

layout(location = 0) out vec4 pixelColor;
layout(location = 1) out vec3 pixelNormal;
layout(location = 2) out float pixelDepth;

void main()
{
	float fow = texture(fowTexture, 1.0 / 34.0 * (fragmentPosition.xz - chunkPosition + 1.0)).r;

	vec3 lightDirection = normalize(vec3(1.0, -1.0, 0.0));
	vec3 directionToCamera = normalize(cameraPosition - fragmentPosition);
	float reflection = max(pow(dot(reflect(lightDirection, fragmentNormal), directionToCamera), 16.0), 0.0) * 0.5;
	reflection = min(pow(reflection, 6) * 64.0, 1.0);

	float light = dot(-fragmentNormal, lightDirection) * 0.5 + 0.5 + reflection;
	pixelColor = vec4(mix(vec3(0.3, 0.6, 0.8) * light, vec3(1.0), reflection) * fow, 0.4 + reflection);
	//pixelColor = vec4(vec3(fragmentPosition.y), 1.0);
	pixelNormal = fragmentNormal * 0.5 + 0.5;
	pixelDepth = gl_FragCoord.z;
}