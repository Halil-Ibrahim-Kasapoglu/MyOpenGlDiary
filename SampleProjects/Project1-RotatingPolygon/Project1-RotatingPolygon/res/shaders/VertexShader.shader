#version 330 core
layout (location = 1) in vec3 aColor;
layout (location = 0) in vec3 aPos;

out vec3 givenColor;

void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
   givenColor = aColor;
}
