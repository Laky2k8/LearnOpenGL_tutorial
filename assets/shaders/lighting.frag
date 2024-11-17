#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;

void main()
{
    FragColor = vec4(lightColor.r * 1.0, lightColor.g * 1.0, lightColor.b * 1.0, 1.0);
}