#version 330 core

out vec4 FragColor;
uniform vec4 bg_color;
in vec3 givenColor;

void main()
{
    FragColor = bg_color;
}
