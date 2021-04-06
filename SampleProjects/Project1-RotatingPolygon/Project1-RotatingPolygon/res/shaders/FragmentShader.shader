#version 330 core

out vec4 FragColor;
uniform vec4 color;
in vec3 givenColor;

void main()
{
    FragColor = vec4(0.75f, 0.45f, 0.05f, 1.0f);
    FragColor = color;
    FragColor = vec4(givenColor.xyz , 1.0f);
}
