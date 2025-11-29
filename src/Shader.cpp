#include "Shader.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCodeStr, fragmentCodeStr;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vShaderFile.close();
    fShaderFile.close();

    vertexCodeStr = vShaderStream.str();
    fragmentCodeStr = fShaderStream.str();

    const char* vertexCode = vertexCodeStr.c_str();
    const char* fragmentCode = fragmentCodeStr.c_str();

    unsigned int vertexShader, fragmentShader;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexCode, NULL);
    glCompileShader(vertexShader);
    this->checkCompilationError(vertexShader, "VERTEX");


    glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
    glCompileShader(fragmentShader);
    this->checkCompilationError(fragmentShader, "FRAGMENT");
    this->m_id = glCreateProgram();

    glAttachShader(this->m_id, vertexShader);
    glAttachShader(this->m_id, fragmentShader);
    glLinkProgram(this->m_id);

}
void Shader::checkCompilationError(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
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

        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
}

void Shader::useProgram()const
{
    glUseProgram(this->m_id);
}
void Shader::setMat4(const glm::mat4 &matrix, const char* uniformName)const
{
    int uniformLocation = glGetUniformLocation(this->m_id, uniformName);

    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
}
void Shader::setVec3(const glm::vec3 &data, const char* uniformName)const
{
    int uniformLocation = glGetUniformLocation(this->m_id, uniformName);

    glUniform3fv(uniformLocation, 1, &data[0]);
}

void Shader::setInt(int data, const char *uniformName)const{
    int uniformLocation = glGetUniformLocation(this->m_id, uniformName);
    glUniform1i(uniformLocation, data);
}
void Shader::setFloat(float data,const char *uniformName)const {
    int uniformLocation = glGetUniformLocation(this->m_id, uniformName);
    glUniform1f(uniformLocation, data);
}
