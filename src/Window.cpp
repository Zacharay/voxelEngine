#include "Window.hpp"
#include <iostream>
#include "Config.hpp"



// Constructor
Window::Window() : m_window(nullptr) {

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(-1);
    }


    m_window = glfwCreateWindow(Config::windowWidth, Config::windowHeight, "OpenGL Window", nullptr, nullptr);
    if (!m_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }


    glfwMakeContextCurrent(m_window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    glViewport(0, 0, Config::windowWidth, Config::windowHeight);

}

// Destructor
Window::~Window() {
    if (m_window) {
        glfwDestroyWindow(m_window);
    }
    glfwTerminate();
}

void Window::run()  {
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(m_window)) {

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glClearColor(0.53f,0.8f,0.93f,1.0f);


        onUpdate();

        onRender();


        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}



