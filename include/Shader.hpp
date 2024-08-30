#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Shader {
    unsigned int ID;
    void checkCompilationError(unsigned int shader, std::string type);
public:
    Shader(const char* vertexSrc, const char* fragmentSrc);
    void useProgram();
    void setMat4(glm::mat4 matrix, const char* uniformName);
    void setVec3(glm::vec3 data, const char* uniformName);
    void setInt(int data, const char* uniformName);
};