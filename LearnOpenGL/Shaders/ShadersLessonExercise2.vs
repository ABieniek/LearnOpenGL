#version 330 core
layout (location = 0) in vec3 aPos;
uniform float fOffset;
out vec3 positionsToColor;
void main()
{
   gl_Position = vec4(-aPos.x + fOffset, -aPos.y, -aPos.z, 1.0);
	positionsToColor = vec3(-aPos.x, -aPos.y, -aPos.z);
}