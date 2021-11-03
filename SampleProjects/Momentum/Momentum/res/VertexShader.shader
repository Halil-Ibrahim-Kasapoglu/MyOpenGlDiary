#version 330 core
layout (location = 0) in vec3 aPos;

uniform vec4 transform;
uniform vec4 scale;

void main()
{
   gl_Position = vec4((aPos.x)*scale.x + transform.x, (aPos.y)*scale.y + transform.y, aPos.z, 1.0);
}
