#version 330

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec3 vertexColor;

out vec3 fragmentPosition;
out vec3 fragmentNormal;
out vec3 fragmentColor;

void main()
{
	fragmentPosition = (vec4(vertexPosition, 1.0)).xyz;
	fragmentNormal = vertexNormal;
	fragmentColor = vertexColor;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}