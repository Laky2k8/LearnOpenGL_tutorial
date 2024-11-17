#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;
    
    void compile(const char* vertexSource, const char* fragmentSource)
    {
        // Create shader objects
        unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
        unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
        
        // Compile vertex shader
        glShaderSource(vertex, 1, &vertexSource, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        
        // Compile fragment shader
        glShaderSource(fragment, 1, &fragmentSource, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        

        // Create shader program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
            
        // Link program
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        
        // Delete shaders after linking
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    // activate the shader
    // ------------------------------------------------------------------------
    void use() const
    { 
        glUseProgram(ID); 
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setFloat(const char *name, float value)
    {

        glUniform1f(glGetUniformLocation(this->ID, name), value);
    }
    void setInt(const char *name, int value)
    {

        glUniform1i(glGetUniformLocation(this->ID, name), value);
    }
    void setVec2f(const char *name, float x, float y)
    {

        glUniform2f(glGetUniformLocation(this->ID, name), x, y);
    }
    void setVec2f(const char *name, const glm::vec2 &value)
    {

        glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
    }
    void setVec3f(const char *name, float x, float y, float z)
    {

        glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
    }
    void setVec3f(const char *name, const glm::vec3 &value)
    {

        glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
    }
    void setVec4f(const char *name, float x, float y, float z, float w)
    {

        glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
    }
    void setVec4f(const char *name, const glm::vec4 &value)
    {

        glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
    }
    void setMat4(const char *name, const glm::mat4 &matrix)
    {

        glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, false, glm::value_ptr(matrix));
    }

private:
    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};
#endif