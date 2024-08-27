#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>
class Window  {
private:
    GLFWwindow *m_window;

public:
    Window();
    void run();
protected:

    ~Window();

    virtual void onUpdate()=0;
    virtual void onRender()=0;

};