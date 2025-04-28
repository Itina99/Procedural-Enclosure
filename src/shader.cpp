//
// Created by Niccolo on 30/03/2025.
//

#include "shader.h"
#include <vector>

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    std::string v_code;
    std::string f_code;

    std::ifstream v_file;
    std::ifstream f_file;

    v_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        v_file.open(vertexPath);
        f_file.open(fragmentPath);
        std::stringstream v_stream, f_stream;
        v_stream << v_file.rdbuf();
        f_stream << f_file.rdbuf();

        v_file.close();
        f_file.close();

        v_code = v_stream.str();
        f_code = f_stream.str();
    }
    catch (std::ifstream::failure e) {
        printf("Impossible to open %s or %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertexPath, fragmentPath);
    }

    // Create the shaders
    GLuint v_shader = glCreateShader(GL_VERTEX_SHADER);
    GLuint f_shader = glCreateShader(GL_FRAGMENT_SHADER);

    GLint result = GL_FALSE;
    int InfoLogLength;

    // Compiling vertex shader
    printf("Compiling shader : %s\n", vertexPath);
    const char * v_source_pointer = v_code.c_str();
    glShaderSource(v_shader, 1, &v_source_pointer, nullptr);
    glCompileShader(v_shader);

    // Checking vertex shader
    glGetShaderiv(v_shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(v_shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if(InfoLogLength > 0) {
        std::vector<char> v_error_message(InfoLogLength + 1);
        glGetShaderInfoLog(v_shader, InfoLogLength, nullptr, &v_error_message[0]);
        printf("%s\n", &v_error_message[0]);
    }

    // Compiling fragment shader
    printf("Compiling shader: %s\n", fragmentPath);
    const char * f_source_pointer = f_code.c_str();
    glShaderSource(f_shader, 1, &f_source_pointer, nullptr);
    glCompileShader(f_shader);
    if(InfoLogLength > 0) {
        std::vector<char> f_error_message(InfoLogLength+1);
        glGetProgramInfoLog(f_shader, InfoLogLength, nullptr, &f_error_message[0]);
        printf("%s\n", &f_error_message[0]);
    }

    // Checking fragment shader
    glGetShaderiv(f_shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(f_shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if(InfoLogLength > 0) {
        std::vector<char> f_error_message(InfoLogLength + 1);
        glGetShaderInfoLog(f_shader, InfoLogLength, nullptr, &f_error_message[0]);
        printf("%s\n", &f_error_message[0]);
    }

    // Linking program
    printf("Linking program\n");
    ID = glCreateProgram();
    glAttachShader(ID, v_shader);
    glAttachShader(ID, f_shader);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &result);
    glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(ID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDetachShader(ID, v_shader);
    glDetachShader(ID, f_shader);

    glDeleteShader(v_shader);
    glDeleteShader(f_shader);
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::unuse() {
    glDeleteProgram(ID);
}


void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string &str,const float x, const float y, const float z) const {
    glUniform3f(glGetUniformLocation(ID, str.c_str()), x, y, z);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}