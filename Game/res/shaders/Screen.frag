#version 330

uniform mat4 inverseViewMatrix;
uniform mat4 inverseProjectionMatrix;

uniform sampler2D renderedColor;
uniform sampler2D renderedNormal;
uniform sampler2D renderedDepth;
uniform sampler2D renderedSSAO;
uniform sampler2D renderedBloom;

in vec2 fragmentTexcoord;

layout(location = 0) out vec4 pixelColor;

float getSmoothSSAO(float area, float step)
{
	vec2 texelSize = 1.0 / textureSize(renderedColor, 0);
	float avarage = 0.0;
	float middle = texture(renderedSSAO, fragmentTexcoord).r;
	vec4 clipSpace = inverseProjectionMatrix * vec4(fragmentTexcoord, texture(renderedDepth, fragmentTexcoord).r, 1.0);
	float distance = clipSpace.z / clipSpace.w;
	vec3 normal = normalize(texture(renderedNormal, fragmentTexcoord).xyz * 2.0 - 1.0);
	float sampleCount = 0.125;
	for (float x = -texelSize.x * area; x <= texelSize.x * area; x += texelSize.x * step) {
		for (float y = -texelSize.y * area; y <= texelSize.y * area; y += texelSize.y * step) {
			vec2 sampleCoord = fragmentTexcoord + vec2(x, y);
			float sample = texture(renderedSSAO, sampleCoord).r;
			vec4 sampleClipSpace = inverseProjectionMatrix * vec4(sampleCoord, texture(renderedDepth, sampleCoord).r, 1.0);
			float sampleDistance = sampleClipSpace.z / sampleClipSpace.w;
			vec3 sampleNormal = normalize(texture(renderedNormal, sampleCoord).xyz * 2.0 - 1.0);
			float normalThreshold = max(dot(sampleNormal, normal), 0.0);
			float threshold = max(1.0 - abs(distance - sampleDistance) * 2.0, 0.0);
			avarage += sample * threshold;
			sampleCount += threshold;
		}
	}
	return avarage / sampleCount;
}

void main()
{	
	pixelColor = vec4(vec3(texture(renderedColor, fragmentTexcoord).xyz * getSmoothSSAO(2.0, 1.0)) + texture(renderedBloom, fragmentTexcoord).xyz * 0.25, 1.0);
}