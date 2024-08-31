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
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

// Destructor
Window::~Window() {
    if (m_window) {
        glfwDestroyWindow(m_window);
    }
    glfwTerminate();
}

void Window::calculateFps() {
    double currentTime = glfwGetTime();
    double elapsedTime = currentTime - previousTime;
    frameCount++;

    if (elapsedTime >= 1.0) { // If a second has passed
        double fps = static_cast<double>(frameCount) / elapsedTime;
        std::cout << "FPS: " << fps << std::endl;

        // Reset for the next calculation
        previousTime = currentTime;
        frameCount = 0;
    }
}



void Window::run()  {
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(m_window)) {

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glClearColor(0.53f,0.8f,0.93f,1.0f);


        onUpdate();

        onRender();

        calculateFps();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}



