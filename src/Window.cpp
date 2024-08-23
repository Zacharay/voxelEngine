#include "Window.hpp"
#include <iostream>

// Constructor
Window::Window() : m_window(nullptr) {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(-1);
    }

    // Create window
    m_window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);
    if (!m_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    // Make the window's context current
    glfwMakeContextCurrent(m_window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
}

// Destructor
Window::~Window() {
    if (m_window) {
        glfwDestroyWindow(m_window);
    }
    glfwTerminate();
}

void Window::run()  {

    while (!glfwWindowShouldClose(m_window)) {

        glClear(GL_COLOR_BUFFER_BIT);

        onUpdate();

        onRender();


        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}



