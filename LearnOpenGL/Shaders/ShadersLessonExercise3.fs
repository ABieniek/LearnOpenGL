#version 330 core
out vec4 FragColor;
in vec3 positionsToColor;
void main()
{
   FragColor = vec4(positionsToColor.xyz, 1.0f);
}