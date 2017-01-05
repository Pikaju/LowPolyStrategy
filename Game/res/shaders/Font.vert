#version 330

uniform mat4 projectionMatrix;

in vec2 vertexPosition;
in vec2 vertexTexcoord;

out vec2 fragmentTexcoord;

void main()
{
	fragmentTexcoord = vertexTexcoord;
	gl_Position = projectionMatrix * vec4(vertexPosition, 0.0, 1.0);
}