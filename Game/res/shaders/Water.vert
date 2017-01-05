#version 330

uniform float time;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

in vec3 vertexPosition;

out vec3 fragmentPosition;
flat out vec3 fragmentNormal;

/*
float octave(float v, float scaleMultiplier, int octaves)
{
	float currentScale = 1.0;
	float result = 0.0;
	for (int i = 0; i < octaves; i++) {
		currentScale *= scaleMultiplier;
		result += sin(currentScale * v) / currentScale;
	}
	return result;
}

float getHeightAt(float x, float y, float time) {
	return (octave(x / 64.0 + octave(y / 48.0 + time / 8.0 + octave((y + x) / 32.0 + time / 8.0, 1.415, 8) / 8.0, 1.98124, 4) / 2.0 + time / 8.0, 2.4, 4));
}
*/

float octave(float v, float scaleMultiplier, int octaves)
{
	float currentScale = 1.0;
	float result = 0.0;
	for (int i = 0; i < octaves; i++) {
		currentScale *= scaleMultiplier;
		result += sin(currentScale * v);
	}
	return result;
}

float getHeightAt(float x, float y, float time) {
	return (octave((x + time + sin(y)) * 1.5, 0.739, 8) * sin(time * 2.1412) + octave((x + y + time + sin(x)) * 1.3, 0.639, 8) * sin(time * 0.7412) + sin((x - y + time) / 4.15612)) * 0.125;
}

void main()
{
	vec3 position = (modelMatrix * vec4(vertexPosition, 1.0)).xyz;
	float h0 = getHeightAt(position.x, position.z, time);
	float h1 = getHeightAt(position.x + 1.0, position.z, time);
	float h2 = getHeightAt(position.x, position.z - 1.0, time);

	vec3 e0 = vec3(position.x + 1.0, h1, position.z) - vec3(position.x, h0, position.z);
	vec3 e1 = vec3(position.x, h2, position.z - 1.0) - vec3(position.x, h0, position.z);
	fragmentNormal = normalize(cross(e0, e1));

	position.y += h0;
	fragmentPosition = position;

	gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0);
}