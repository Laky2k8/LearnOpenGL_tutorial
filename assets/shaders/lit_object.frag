#version 330 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 normal;
in vec3 fragPos;

void main()
{

    //Ambient settings
    float ambientStrength = 0.2;

    //Specular settings
    float specularStrength = 0.5;
    float specularShininess = 128;

    //We be ambient lighting-ing with this one
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);

    //It's diffuse lighting time my ninjas
    float diff = max(dot(norm, lightDir), 0.0); // We are max-ing it so that it cannot be negative, cuz negative colors don't exist dumbass
    vec3 diffuse = diff * lightColor;

    //And to top it all off, specular lighting
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectionDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0), specularShininess);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}