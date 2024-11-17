# version 330 core

layout (location = 0) in vec3 aPos; // The position variable has attribute position 0
layout (location = 1) in vec3 aNormal;
//layout (location = 1) in vec2 aTexCoord; // The texture variable has attribute position 2

out vec3 vertColor;
//out vec2 texCoord;
out vec3 fragPos;  
out vec3 normal;

// Matrices for 3D rendering
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // note that we read the multiplication from right to left
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    fragPos = vec3(model * vec4(aPos, 1.0));

    normal = mat3(transpose(inverse(model))) * aNormal;  
}