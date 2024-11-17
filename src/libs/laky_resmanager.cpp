// LAKY'S RESOURCE MANAGER v1.0.0
// 2024.11.01.
//==============================================================================

#include "laky_resmanager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Instantiate static variables
std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>       ResourceManager::Shaders;


Shader ResourceManager::LoadShader(const char *vShaderFile, const char *fShaderFile, std::string name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile);
    return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const char *file, bool alpha, std::string name)
{
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

void ResourceManager::Clear()
{
    // (properly) delete all shaders	
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.ID);
    // (properly) delete all textures
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const char *vShaderFile, const char *fShaderFile)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        
        if (!vertexShaderFile.is_open()) {
            std::cout << "Failed to open vertex shader file: " << vShaderFile << std::endl;
            throw std::runtime_error("Vertex shader file not found");
        }
        if (!fragmentShaderFile.is_open()) {
            std::cout << "Failed to open fragment shader file: " << fShaderFile << std::endl;
            throw std::runtime_error("Fragment shader file not found");
        }

        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        
        // Debug print
        std::cout << "Loaded vertex shader:\n" << vertexCode << std::endl;
        std::cout << "Loaded fragment shader:\n" << fragmentCode << std::endl;
    }
    catch (std::exception& e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files: " << e.what() << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    
    // 2. now create shader object from source code
    Shader shader;
    shader.compile(vShaderCode, fShaderCode);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char *file, bool alpha)
{
    Texture2D texture;
    
    
    // load image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    
    if (data == NULL) {
        std::cout << "Failed to load texture: " << file << std::endl;
        std::cout << "STB Reason: " << stbi_failure_reason() << std::endl;
        return texture;
    }

    // Set format based on number of channels
    if (nrChannels == 4 || alpha) {
        texture.internal_format = GL_RGBA;
        texture.image_format = GL_RGBA;
    } else if (nrChannels == 3) {
        texture.internal_format = GL_RGB;
        texture.image_format = GL_RGB;
    }

    texture.Generate(width, height, data);
    stbi_image_free(data);
    return texture;
}