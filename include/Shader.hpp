#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Shader {

public:
    Shader(const char* vertexSrc, const char* fragmentSrc);
    void useProgram()const;

    void setMat4(const glm::mat4 &matrix, const char* uniformName)const;
    void setVec3(const glm::vec3 &data, const char* uniformName)const;
    void setInt(int data, const char* uniformName)const;
    void setFloat(float data,const char *uniformName)const;
private:
    unsigned int m_id;
    void checkCompilationError(unsigned int shader, std::string type);
};