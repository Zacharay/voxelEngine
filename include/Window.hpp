#include <glad/glad.h>
#include <GLFW/glfw3.h>

#pragma once


class Window  {
private:
    GLFWwindow *m_window;
    ~Window();
public:
    Window();

protected:


    void run() ;
    virtual void onUpdate()=0;
    virtual void onRender()=0;

};