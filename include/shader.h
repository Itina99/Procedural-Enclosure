//
// Created by Niccolo on 30/03/2025.
//

#ifndef SHADER_H
#define SHADER_H



#include <glad/glad.h>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>

class Shader {
public:
    // Program ID
    unsigned int ID;

    // Constructor
    Shader(const char* vertexPath, const char* fragmentPath);

    // activate the shader
    void use();

    // delete shader
    void unuse();

    // Utils
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setVec3(const std::string& str, float x, float y, float z)const;
};




#endif //SHADER_H
