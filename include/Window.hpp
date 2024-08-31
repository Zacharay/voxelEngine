#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>
class Window  {
private:
    void calculateFps();
    int frameCount;
    double previousTime;
public:
    Window();
    void run();
protected:
    GLFWwindow *m_window;
    ~Window();


    virtual void onUpdate()=0;
    virtual void onRender()=0;

};